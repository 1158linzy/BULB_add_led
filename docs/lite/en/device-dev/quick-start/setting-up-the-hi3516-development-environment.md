# Setting Up the Hi3516 Development Environment<a name="EN-US_TOPIC_0000001054501981"></a>

-   [Prerequisites](#section2074878255)
-   [Requirements Specific to the Hi3516 Development Environment](#section179175261196)
    -   [Hardware Requirements](#section5840424125014)
    -   [Software Requirements](#section965634210501)

-   [Installing the Linux Compilation Environment](#section182916865219)
    -   [Changing Linux Shell to Bash](#section1715027152617)
    -   [Installing Basic Software Used for Compilation and Building \(Required Only for Ubuntu 20+\)](#section45512412251)
    -   [Installing a File Packing Tool](#section8352161417450)
    -   [Installing hc-gen](#section2075110583451)
    -   [Installing the JVM](#section101989994613)


## Prerequisites<a name="section2074878255"></a>

You must complete the settings in  [Basic OS Environment Setup](basic-os-environment-setup.md).

## Requirements Specific to the Hi3516 Development Environment<a name="section179175261196"></a>

### Hardware Requirements<a name="section5840424125014"></a>

-   Hi3516D V300 IoT camera development board
-   USB-to-serial cable and network cable \(The Windows workstation is connected to Hi3516D V300 development board through the USB-to-serial cable and network cable.\)

The following figure shows the hardware connections.

**Figure  1**  Hardware connections<a name="fig19527104710591"></a>  


![](figures/矩形备份-292.png)

### Software Requirements<a name="section965634210501"></a>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>This section describes how to use an installation package to set up the compilation and development environment. If you are going to use Docker to set up the environment, skip this section and  [Installing the Linux Compilation Environment](#section182916865219).

The following table describes the tools required for setting up the general environment for a Linux server of the Hi3516 development board and how to obtain these tools.

**Table  1**  Development tools and obtaining methods

<a name="table6299192712513"></a>
<table><thead align="left"><tr id="row122993276512"><th class="cellrowborder" valign="top" width="13.081308130813083%" id="mcps1.2.4.1.1"><p id="p1829914271858"><a name="p1829914271858"></a><a name="p1829914271858"></a>Development Tool</p>
</th>
<th class="cellrowborder" valign="top" width="19.921992199219922%" id="mcps1.2.4.1.2"><p id="p429918274517"><a name="p429918274517"></a><a name="p429918274517"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="66.996699669967%" id="mcps1.2.4.1.3"><p id="p12997271757"><a name="p12997271757"></a><a name="p12997271757"></a>How to Obtain</p>
</th>
</tr>
</thead>
<tbody><tr id="row167343191518"><td class="cellrowborder" valign="top" width="13.081308130813083%" headers="mcps1.2.4.1.1 "><p id="p467443191517"><a name="p467443191517"></a><a name="p467443191517"></a>bash</p>
</td>
<td class="cellrowborder" valign="top" width="19.921992199219922%" headers="mcps1.2.4.1.2 "><p id="p0674153114151"><a name="p0674153114151"></a><a name="p0674153114151"></a>Processes CLI commands.</p>
</td>
<td class="cellrowborder" valign="top" width="66.996699669967%" headers="mcps1.2.4.1.3 "><p id="p116746312151"><a name="p116746312151"></a><a name="p116746312151"></a>System configuration</p>
</td>
</tr>
<tr id="row14885193315201"><td class="cellrowborder" valign="top" width="13.081308130813083%" headers="mcps1.2.4.1.1 "><p id="p137174662119"><a name="p137174662119"></a><a name="p137174662119"></a>Basic software package for compilation and building (required only for Ubuntu 20+)</p>
</td>
<td class="cellrowborder" valign="top" width="19.921992199219922%" headers="mcps1.2.4.1.2 "><p id="p258814561424"><a name="p258814561424"></a><a name="p258814561424"></a>Provides a basic software package for compilation and building.</p>
</td>
<td class="cellrowborder" valign="top" width="66.996699669967%" headers="mcps1.2.4.1.3 "><p id="p1749611716181"><a name="p1749611716181"></a><a name="p1749611716181"></a>Internet</p>
</td>
</tr>
<tr id="row52253812238"><td class="cellrowborder" valign="top" width="13.081308130813083%" headers="mcps1.2.4.1.1 "><p id="p28007392236"><a name="p28007392236"></a><a name="p28007392236"></a>dosfstools, mtools, and mtd-utils</p>
</td>
<td class="cellrowborder" valign="top" width="19.921992199219922%" headers="mcps1.2.4.1.2 "><p id="p98008390232"><a name="p98008390232"></a><a name="p98008390232"></a>Pack files.</p>
</td>
<td class="cellrowborder" valign="top" width="66.996699669967%" headers="mcps1.2.4.1.3 "><p id="p280018394233"><a name="p280018394233"></a><a name="p280018394233"></a>apt-get install</p>
</td>
</tr>
<tr id="row18516509507"><td class="cellrowborder" valign="top" width="13.081308130813083%" headers="mcps1.2.4.1.1 "><p id="p14521650135017"><a name="p14521650135017"></a><a name="p14521650135017"></a>hc-gen</p>
</td>
<td class="cellrowborder" valign="top" width="19.921992199219922%" headers="mcps1.2.4.1.2 "><p id="p125255010501"><a name="p125255010501"></a><a name="p125255010501"></a>Configures and compiles files.</p>
</td>
<td class="cellrowborder" valign="top" width="66.996699669967%" headers="mcps1.2.4.1.3 "><p id="p145061517114519"><a name="p145061517114519"></a><a name="p145061517114519"></a><a href="https://repo.huaweicloud.com/harmonyos/compiler/hc-gen/0.65/linux/hc-gen-0.65-linux.tar" target="_blank" rel="noopener noreferrer">https://repo.huaweicloud.com/harmonyos/compiler/hc-gen/0.65/linux/hc-gen-0.65-linux.tar</a></p>
</td>
</tr>
<tr id="row29204072315"><td class="cellrowborder" valign="top" width="13.081308130813083%" headers="mcps1.2.4.1.1 "><p id="p5921190162318"><a name="p5921190162318"></a><a name="p5921190162318"></a>Java virtual machine (JVM)</p>
</td>
<td class="cellrowborder" valign="top" width="19.921992199219922%" headers="mcps1.2.4.1.2 "><p id="p17921110152311"><a name="p17921110152311"></a><a name="p17921110152311"></a>Compiles, debugs, and runs Java programs.</p>
</td>
<td class="cellrowborder" valign="top" width="66.996699669967%" headers="mcps1.2.4.1.3 "><p id="p16921805237"><a name="p16921805237"></a><a name="p16921805237"></a>apt-get install</p>
</td>
</tr>
</tbody>
</table>

## Installing the Linux Compilation Environment<a name="section182916865219"></a>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>-   If you acquire the source code using an HPM component or HPM CLI tool, compilation tools like  **LLVM**  and  **hc-gen**  are not required.
>-   \(Recommended\) If you obtain the source code through an image site or a code repository, install compilation tools like  **LLVM**  and  **hc-gen**. When installing  **LLVM**  and  **hc-gen**, ensure that the environment variable paths of the compilation tools are unique.

### Changing Linux Shell to Bash<a name="section1715027152617"></a>

Check whether bash is used as the shell.

```
ls -l /bin/sh
```

If  **/bin/sh -\> bash**  is not displayed, do as follows to change shell to bash.

**Method 1:**  Run the following command on the device and then click  **No**.

```
sudo dpkg-reconfigure dash
```

**Method 2:**  Run the first command to delete  **sh**  and then run the second command to create a new soft link.

```
sudo rm -rf /bin/sh
sudo ln -s /bin/bash /bin/sh
```

### Installing Basic Software Used for Compilation and Building \(Required Only for Ubuntu 20+\)<a name="section45512412251"></a>

Install the software.

```
sudo apt-get install build-essential && sudo apt-get install gcc && sudo apt-get install g++ && sudo apt-get install make && sudo apt-get install zlib* && sudo apt-get install libffi-dev
```

### Installing a File Packing Tool<a name="section8352161417450"></a>

1.  Start a Linux server.
2.  Install  **dosfstools**.

    ```
    sudo apt-get install dosfstools
    ```

3.  Install  **mtools**.

    ```
    sudo apt-get install mtools
    ```

4.  Install  **mtd-utils**.

    ```
    sudo apt-get install mtd-utils
    ```


### Installing hc-gen<a name="section2075110583451"></a>

1.  Start a Linux server.
2.  Download  [hc-gen](https://repo.huaweicloud.com/harmonyos/compiler/hc-gen/0.65/linux/hc-gen-0.65-linux.tar).
3.  Decompress the hc-gen installation package to  **\~/hc-gen**  on the Linux server.

    ```
    tar -xvf hc-gen-0.65-linux.tar -C ~/
    ```

4.  Set an environment variable.

    ```
    vim ~/.bashrc
    ```

    Copy the following command to the last line of the  **.bashrc**  file, save the file, and exit.

    ```
    export PATH=~/hc-gen:$PATH
    ```

5.  Validate the environment variable.

    ```
    source ~/.bashrc
    ```


### Installing the JVM<a name="section101989994613"></a>

1.  Start a Linux server.
2.  Install the Java Runtime Environment \(JRE\).

    ```
    sudo apt-get install default-jre
    ```

3.  Install the Java Development Kit \(JDK\).

    ```
    sudo apt-get install default-jdk
    ```


