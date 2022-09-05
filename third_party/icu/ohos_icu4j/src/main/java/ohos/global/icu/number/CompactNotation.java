/* GENERATED SOURCE. DO NOT MODIFY. */
// © 2017 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html#License
package ohos.global.icu.number;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import ohos.global.icu.impl.StandardPlural;
import ohos.global.icu.impl.number.CompactData;
import ohos.global.icu.impl.number.CompactData.CompactType;
import ohos.global.icu.impl.number.DecimalFormatProperties;
import ohos.global.icu.impl.number.DecimalQuantity;
import ohos.global.icu.impl.number.MicroProps;
import ohos.global.icu.impl.number.MicroPropsGenerator;
import ohos.global.icu.impl.number.MutablePatternModifier;
import ohos.global.icu.impl.number.MutablePatternModifier.ImmutablePatternModifier;
import ohos.global.icu.impl.number.PatternStringParser;
import ohos.global.icu.impl.number.PatternStringParser.ParsedPatternInfo;
import ohos.global.icu.text.CompactDecimalFormat.CompactStyle;
import ohos.global.icu.text.NumberFormat;
import ohos.global.icu.text.PluralRules;
import ohos.global.icu.util.ULocale;

/**
 * A class that defines the scientific notation style to be used when formatting numbers in
 * NumberFormatter.
 *
 * <p>
 * This class exposes no public functionality. To create a CompactNotation, use one of the factory
 * methods in {@link Notation}.
 *
 * @see NumberFormatter
 * @hide exposed on OHOS
 */
public class CompactNotation extends Notation {

    final CompactStyle compactStyle;
    final Map<String, Map<String, String>> compactCustomData;

    /**
     * Create a compact notation with custom data.
     * @deprecated This API is ICU internal only.
     * @see DecimalFormatProperties#setCompactCustomData
     * @hide draft / provisional / internal are hidden on OHOS
     */
    @Deprecated
    public static CompactNotation forCustomData(Map<String, Map<String, String>> compactCustomData) {
        return new CompactNotation(compactCustomData);
    }

    /* package-private */ CompactNotation(CompactStyle compactStyle) {
        compactCustomData = null;
        this.compactStyle = compactStyle;
    }

    /* package-private */ CompactNotation(Map<String, Map<String, String>> compactCustomData) {
        compactStyle = null;
        this.compactCustomData = compactCustomData;
    }

    /* package-private */ MicroPropsGenerator withLocaleData(
            ULocale locale,
            String nsName,
            CompactType compactType,
            PluralRules rules,
            MutablePatternModifier buildReference,
            boolean safe,
            MicroPropsGenerator parent) {
        // TODO: Add a data cache? It would be keyed by locale, nsName, compact type, and compact style.
        return new CompactHandler(this, locale, nsName, compactType, rules, buildReference, safe, parent);
    }

    private static class CompactHandler implements MicroPropsGenerator {

        final PluralRules rules;
        final MicroPropsGenerator parent;
        final Map<String, ImmutablePatternModifier> precomputedMods;
        final MutablePatternModifier unsafePatternModifier;
        final CompactData data;

        private CompactHandler(
                CompactNotation notation,
                ULocale locale,
                String nsName,
                CompactType compactType,
                PluralRules rules,
                MutablePatternModifier buildReference,
                boolean safe,
                MicroPropsGenerator parent) {
            this.rules = rules;
            this.parent = parent;
            this.data = new CompactData();
            if (notation.compactStyle != null) {
                data.populate(locale, nsName, notation.compactStyle, compactType);
            } else {
                data.populate(notation.compactCustomData);
            }
            if (safe) {
                // Safe code path
                precomputedMods = new HashMap<>();
                precomputeAllModifiers(buildReference);
                unsafePatternModifier = null;
            } else {
                // Unsafe code path
                precomputedMods = null;
                unsafePatternModifier = buildReference;
            }
        }

        /** Used by the safe code path */
        private void precomputeAllModifiers(MutablePatternModifier buildReference) {
            Set<String> allPatterns = new HashSet<>();
            data.getUniquePatterns(allPatterns);

            for (String patternString : allPatterns) {
                ParsedPatternInfo patternInfo = PatternStringParser.parseToPatternInfo(patternString);
                buildReference.setPatternInfo(patternInfo, NumberFormat.Field.COMPACT);
                precomputedMods.put(patternString, buildReference.createImmutable());
            }
        }

        @Override
        public MicroProps processQuantity(DecimalQuantity quantity) {
            MicroProps micros = parent.processQuantity(quantity);
            assert micros.rounder != null;

            // Treat zero, NaN, and infinity as if they had magnitude 0
            int magnitude;
            int multiplier = 0;
            if (quantity.isZeroish()) {
                magnitude = 0;
                micros.rounder.apply(quantity);
            } else {
                multiplier = micros.rounder.chooseMultiplierAndApply(quantity, data);
                magnitude = quantity.isZeroish() ? 0 : quantity.getMagnitude();
                magnitude -= multiplier;
            }

            StandardPlural plural = quantity.getStandardPlural(rules);
            String patternString = data.getPattern(magnitude, plural);
            if (patternString == null) {
                // Use the default (non-compact) modifier.
                // No need to take any action.
            } else if (precomputedMods != null) {
                // Safe code path.
                // Java uses a hash set here for O(1) lookup. C++ uses a linear search.
                ImmutablePatternModifier mod = precomputedMods.get(patternString);
                mod.applyToMicros(micros, quantity);
            } else {
                // Unsafe code path.
                // Overwrite the PatternInfo in the existing modMiddle.
                ParsedPatternInfo patternInfo = PatternStringParser.parseToPatternInfo(patternString);
                unsafePatternModifier.setPatternInfo(patternInfo, NumberFormat.Field.COMPACT);
                unsafePatternModifier.setNumberProperties(quantity.signum(), null);
                micros.modMiddle = unsafePatternModifier;
            }

            // Change the exponent only after we select appropriate plural form
            // for formatting purposes so that we preserve expected formatted
            // string behavior.
            quantity.adjustExponent(-1 * multiplier);

            // We already performed rounding. Do not perform it again.
            micros.rounder = null;

            return micros;
        }
    }
}