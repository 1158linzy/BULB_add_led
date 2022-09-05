/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UNIT_NUMBER_TEST_H
#define UNIT_NUMBER_TEST_H

#include <string>
#include <vector>
#include <algorithm>
#include "locale_info.h"
#include "number_format.h"

namespace testing {
using namespace OHOS;
using ::std::string;
using ::std::vector;
using namespace I18N;
vector<LocaleInfo> g_locales = {
    LocaleInfo("ar", "", ""),       LocaleInfo("ar", "AE"),         LocaleInfo("ar", "EG"),
    LocaleInfo("as", "Latn", "IN"), LocaleInfo("de", "DE"),         LocaleInfo("en", "", ""),
    LocaleInfo("en", "GB"),         LocaleInfo("en", "Qaag", ""),   LocaleInfo("en", "US"),
    LocaleInfo("es", "ES"),         LocaleInfo("es", "US"),         LocaleInfo("fr", "FR"),
    LocaleInfo("it", "IT"),         LocaleInfo("ka", "GE"),         LocaleInfo("mai", "Deva", ""),
    LocaleInfo("my", "MM"),         LocaleInfo("pa", "Guru", ""),   LocaleInfo("pt", "BR"),
    LocaleInfo("pt", "PT"),         LocaleInfo("sr", "Cyrl", ""),   LocaleInfo("th", "TH"),
    LocaleInfo("zh", "", ""),       LocaleInfo("zh", "Hans", ""),   LocaleInfo("zh", "Hans", "CN"),
    LocaleInfo("zh", "Hans", "HK"), LocaleInfo("zh", "Hans", "MO"), LocaleInfo("zh", "Hant", "TW")
};

string g_numberFormExpect[][20] = {
    { "١٬٢٣٤٬٥٦٧", "١٢٣٬٤٥٦٬٧٨٩", "١٬٢٣٤",
      "١٢٣٤٥٦٧", "١٢٣٤٥٦٧٨٩", "١٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٦٨", "٨٢٣٬٤٥٦٬٧٨٩٫٥٦٧", "٨٬٢٣٤",
      "٨٢٣٤٥٦٧٫٥٦٨", "٨٢٣٤٥٦٧٨٩٫٥٦٧", "٨٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٧", "٨٢٣٬٤٥٦٬٧٨٩٫٥٧",
      "٨٬٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٦٧٨", "٨٢٣٬٤٥٦٬٧٨٩٫٥٦٧", "٨٬٢٣٤٫٠٠٠", "١٢٪؜‏", "١٢٪؜‏" },
    { "١٬٢٣٤٬٥٦٧", "١٢٣٬٤٥٦٬٧٨٩", "١٬٢٣٤", 
      "١٢٣٤٥٦٧", "١٢٣٤٥٦٧٨٩", "١٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٦٨", "٨٢٣٬٤٥٦٬٧٨٩٫٥٦٧", "٨٬٢٣٤", 
      "٨٢٣٤٥٦٧٫٥٦٨", "٨٢٣٤٥٦٧٨٩٫٥٦٧", "٨٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٧", "٨٢٣٬٤٥٦٬٧٨٩٫٥٧",
      "٨٬٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٦٧٨", "٨٢٣٬٤٥٦٬٧٨٩٫٥٦٧", "٨٬٢٣٤٫٠٠٠", "١٢٪؜‏", "١٢٪؜‏" },
    { "١٬٢٣٤٬٥٦٧", "١٢٣٬٤٥٦٬٧٨٩", "١٬٢٣٤",
      "١٢٣٤٥٦٧", "١٢٣٤٥٦٧٨٩", "١٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٦٨", "٨٢٣٬٤٥٦٬٧٨٩٫٥٦٧", "٨٬٢٣٤",
      "٨٢٣٤٥٦٧٫٥٦٨", "٨٢٣٤٥٦٧٨٩٫٥٦٧", "٨٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٧", "٨٢٣٬٤٥٦٬٧٨٩٫٥٧",
      "٨٬٢٣٤", "٨٬٢٣٤٬٥٦٧٫٥٦٧٨", "٨٢٣٬٤٥٦٬٧٨٩٫٥٦٧", "٨٬٢٣٤٫٠٠٠", "١٢٪؜‏", "١٢٪؜‏" },
    { "১২,৩৪,৫৬৭", "১২,৩৪,৫৬,৭৮৯", "১,২৩৪", "১২৩৪৫৬৭", "১২৩৪৫৬৭৮৯", "১২৩৪",
      "৮২,৩৪,৫৬৭.৫৬৮", "৮২,৩৪,৫৬,৭৮৯.৫৬৭", "৮,২৩৪", "৮২৩৪৫৬৭.৫৬৮", "৮২৩৪৫৬৭৮৯.৫৬৭",
      "৮২৩৪", "৮২,৩৪,৫৬৭.৫৭", "৮২,৩৪,৫৬,৭৮৯.৫৭", "৮,২৩৪", "৮২,৩৪,৫৬৭.৫৬৭৮",
      "৮২,৩৪,৫৬,৭৮৯.৫৬৭", "৮,২৩৪.০০০", "১২%", "১২%" },
    { "1.234.567", "123.456.789", "1.234", "1234567", "123456789", "1234", "8.234.567,568",
      "823.456.789,567", "8.234", "8234567,568", "823456789,567", "8234", "8.234.567,57",
      "823.456.789,57", "8.234", "8.234.567,5678", "823.456.789,567", "8.234,000", "12 %", "12 %" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1.234.567", "123.456.789", "1.234", "1234567", "123456789", "1234", "8.234.567,568",
      "823.456.789,567", "8.234", "8234567,568", "823456789,567", "8234", "8.234.567,57",
      "823.456.789,57", "8.234", "8.234.567,5678", "823.456.789,567", "8.234,000", "12 %", "12 %" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12 %", "12 %" },
    { "1 234 567", "123 456 789", "1 234", "1234567", "123456789", "1234", "8 234 567,568",
      "823 456 789,567", "8 234", "8234567,568", "823456789,567", "8234", "8 234 567,57",
      "823 456 789,57", "8 234", "8 234 567,5678", "823 456 789,567", "8 234,000", "12 %", "12 %" },
    { "1.234.567", "123.456.789", "1.234", "1234567", "123456789", "1234", "8.234.567,568",
      "823.456.789,567", "8.234", "8234567,568", "823456789,567", "8234", "8.234.567,57",
      "823.456.789,57", "8.234", "8.234.567,5678", "823.456.789,567", "8.234,000", "12%", "12%" },
    { "1 234 567", "123 456 789", "1 234", "1234567", "123456789", "1234", "8 234 567,568",
      "823 456 789,567", "8 234", "8234567,568", "823456789,567", "8234", "8 234 567,57",
      "823 456 789,57", "8 234", "8 234 567,5678", "823 456 789,567", "8 234,000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "၁,၂၃၄,၅၆၇", "၁၂၃,၄၅၆,၇၈၉", "၁,၂၃၄", "၁၂၃၄၅၆၇", "၁၂၃၄၅၆၇၈၉", "၁၂၃၄", "၈,၂၃၄,၅၆၇.၅၆၈",
      "၈၂၃,၄၅၆,၇၈၉.၅၆၇", "၈,၂၃၄", "၈၂၃၄၅၆၇.၅၆၈", "၈၂၃၄၅၆၇၈၉.၅၆၇", "၈၂၃၄", "၈,၂၃၄,၅၆၇.၅၇",
      "၈၂၃,၄၅၆,၇၈၉.၅၇", "၈,၂၃၄", "၈,၂၃၄,၅၆၇.၅၆၇၈", "၈၂၃,၄၅၆,၇၈၉.၅၆၇", "၈,၂၃၄.၀၀၀", "၁၂%", "၁၂%" },
    { "12,34,567", "12,34,56,789", "1,234", "1234567", "123456789", "1234", "82,34,567.568",
      "82,34,56,789.567", "8,234", "8234567.568", "823456789.567", "8234", "82,34,567.57",
      "82,34,56,789.57", "8,234", "82,34,567.5678", "82,34,56,789.567", "8,234.000", "12%", "12%" },
    { "1.234.567", "123.456.789", "1.234", "1234567", "123456789", "1234", "8.234.567,568",
      "823.456.789,567", "8.234", "8234567,568", "823456789,567", "8234", "8.234.567,57",
      "823.456.789,57", "8.234", "8.234.567,5678", "823.456.789,567", "8.234,000", "12%", "12%" },
    { "1 234 567", "123 456 789", "1 234", "1234567", "123456789", "1234", "8 234 567,568",
      "823 456 789,567", "8 234", "8234567,568", "823456789,567", "8234", "8 234 567,57",
      "823 456 789,57", "8 234", "8 234 567,5678", "823 456 789,567", "8 234,000", "12%", "12%" },
    { "1.234.567", "123.456.789", "1.234", "1234567", "123456789", "1234", "8.234.567,568",
      "823.456.789,567", "8.234", "8234567,568", "823456789,567", "8234", "8.234.567,57",
      "823.456.789,57", "8.234", "8.234.567,5678", "823.456.789,567", "8.234,000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" },
    { "1,234,567", "123,456,789", "1,234", "1234567", "123456789", "1234", "8,234,567.568",
      "823,456,789.567", "8,234", "8234567.568", "823456789.567", "8234", "8,234,567.57",
      "823,456,789.57", "8,234", "8,234,567.5678", "823,456,789.567", "8,234.000", "12%", "12%" }
};
} // namespace testing

#endif // UNIT_NUMBER_TEST_H
