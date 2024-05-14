# Bluetooth&reg; LE CTS Server

This code example demonstrates the implementation of a simple AIROC&trade; Bluetooth&reg; LE GAP Central - GATT Server with Current Time Service ([CTS](https://www.bluetooth.com/specifications/specs/current-time-service-1-1)) using  PSoC&trade;  6 MCU, AIROC&trade;  CYW20829, and ModusToolbox&trade;  software environment.

In this code example, the kit scans for "CTS Client", and after connection with a CTS-based client, sends notifications with Current Time characteristic values derived from the local date and time. The time and date sent as notification are printed on the serial terminal.

This code example along with Bluetooth&reg; LE CTS Client CE [Bluetooth&reg; LE CTS Client](https://github.com/Infineon/mtb-example-btstack-freertos-cts-client) are low-power-enabled for AIROC&trade; Bluetooth&reg; LE and can be used to measure the current consumption by PSoC&trade; 6 (not optimized for power) and CYW43XXX when using the AIROC&trade; Bluetooth&reg; LE feature. See [AN227910: Low-power system design with CYW43012 and PSoC&trade; 6 MCU](https://www.infineon.com/dgdl/Infineon-AN227910_Low-power_system_design_with_AIROC_CYW43012_Wi-Fi_%26_Bluetooth_combo_chip_and_PSoC_6_MCU-ApplicationNotes-v03_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d39b66166f3) to learn about Bluetooth&reg; power optimization techniques and power measurement using this CE.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-btstack-freertos-cts-server)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzAzMDIiLCJTcGVjIE51bWJlciI6IjAwMi0zMDMwMiIsIkRvYyBUaXRsZSI6IkJsdWV0b290aCZyZWc7IExFIENUUyBTZXJ2ZXIiLCJyaWQiOiJkcmFqIiwiRG9jIHZlcnNpb24iOiI0LjcuMCIsIkRvYyBMYW5ndWFnZSI6IkVuZ2xpc2giLCJEb2MgRGl2aXNpb24iOiJNQ0QiLCJEb2MgQlUiOiJJQ1ciLCJEb2MgRmFtaWx5IjoiQlRBQkxFIn0=)

## Requirements

- [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) v3.1 or later (tested with v3.1)
- Board support package (BSP) minimum required version for:
   - PSoC&trade; 6 MCU: v4.0.0
   - CYW920829M2EVK-02: v1.0.2
   - CYW989829M2EVB-01: v1.0.1
- Programming language: C
- Associated parts: All [PSoC&trade; 6 MCU](https://www.infineon.com/cms/en/product/microcontroller/32-bit-psoc-arm-cortex-microcontroller/psoc-6-32-bit-arm-cortex-m4-mcu/) with [AIROC™ CYW43012 Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/cyw43012), [AIROC™ CYW4343W Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/cyw4343w), [AIROC™ CYW43438 Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/cyw43438), [AIROC™ CYW4373 Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/cyw4373),[AIROC&trade; CYW43022 Wi-Fi & Bluetooth&reg; combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/wi-fi-5-802.11ac/cyw43022)

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg; Embedded Compiler v11.3.1 (`GCC_ARM`) – Default value of `TOOLCHAIN`
- Arm&reg; Compiler v6.16 (`ARM`)
- IAR C/C++ Compiler v9.30.1 (`IAR`)

## Supported kits (make variable 'TARGET')

- [PSoC&trade; 6 Wi-Fi Bluetooth&reg; Prototyping Kit](https://www.infineon.com/CY8CPROTO-062-4343W) (`CY8CPROTO-062-4343W`) – Default value of `TARGET`
- [AIROC&trade; CYW20829 Bluetooth&reg; LE Evaluation Kit](https://www.infineon.com/CYW920829M2EVK-02) (`CYW920829M2EVK-02`)
- [AIROC&trade; CYW89829 Bluetooth&reg; LE Evaluation Kit]() (`CYW989829M2EVB-01`)
- [PSoC&trade; 6 Wi-Fi Bluetooth&reg; Pioneer Kit](https://www.infineon.com/CY8CKIT-062-WIFI-BT) (`CY8CKIT-062-WIFI-BT`)
- [PSoC&trade; 62S2 Wi-Fi Bluetooth&reg; Pioneer Kit](https://www.infineon.com/CY8CKIT-062S2-43012) (`CY8CKIT-062S2-43012`)
- [PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; Pioneer Kit](https://www.infineon.com/CYW9P62S1-43438EVB-01) (`CYW9P62S1-43438EVB-01`)
- [PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; Pioneer Kit](https://www.infineon.com/CYW9P62S1-43012EVB-01) (`CYW9P62S1-43012EVB-01`)
- [PSoC&trade; 62S3 Wi-Fi Bluetooth&reg; Prototyping Kit](https://www.infineon.com/CY8CPROTO-062S3-4343W) (`CY8CPROTO-062S3-4343W`)
- [PSoC&trade; 64 "Secure Boot" Wi-Fi Bluetooth&reg; Pioneer Kit](https://www.infineon.com/CY8CKIT-064B0S2-4343W) (`CY8CKIT-064B0S2-4343W`)
- [PSoC&trade; 62S2 Evaluation Kit](https://www.infineon.com/CY8CEVAL-062S2) (`CY8CEVAL-062S2`, `CY8CEVAL-062S2-LAI-4373M2`, `CY8CEVAL-062S2-MUR-43439M2`, `CY8CEVAL-062S2-LAI-43439M2`, `CY8CEVAL-062S2-MUR-4373EM2`, `CY8CEVAL-062S2-MUR-4373M2`, `CY8CEVAL-062S2-CYW43022CUB` )
- [PSoC&trade; 6 Bluetooth&reg; LE Pioneer Kit](https://www.infineon.com/CY8CKIT-062-BLE) (`CY8CKIT-062-BLE`)
- [PSoC&trade; 6 Bluetooth&reg; LE Prototyping Kit](https://www.infineon.com/CY8CPROTO-063-BLE) (`CY8CPROTO-063-BLE`)
- [EZ-BLE Arduino Evaluation Board](https://www.infineon.com/cms/en/product/evaluation-boards/cyble-416045-eval/) (`CYBLE-416045-EVAL`)
- [PSoC&trade; 62S2 Wi-Fi Bluetooth&reg; Prototyping Kit](https://www.infineon.com/CY8CPROTO-062-43439) (`CY8CPROTO-062S2-43439`)
- [PSoC&trade; 6 AI Evaluation Kit](https://www.infineon.com/CY8CKIT-062S2-AI) (`CY8CKIT-062S2-AI`)

## Hardware setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.

**Note:** The PSoC&trade; 6 Bluetooth&reg; LE Pioneer Kit (CY8CKIT-062-BLE) and the PSoC&trade; 6 Wi-Fi Bluetooth&reg; Pioneer Kit (CY8CKIT-062-WIFI-BT) ship with KitProg2 installed. The ModusToolbox&trade; software requires KitProg3. Before using this code example, make sure that the board is upgraded to KitProg3. The tool and instructions are available in the [Firmware Loader](https://github.com/Infineon/Firmware-loader) GitHub repository. If you do not upgrade, you will see an error like "unable to find CMSIS-DAP device" or "KitProg firmware is out of date".

The AIROC&trade; CYW20829 Bluetooth&reg; kit (CYW920829M2EVK-02) ships with KitProg3 version 2.21 installed. The ModusToolbox™ software requires KitProg3 with latest version 2.40. Before using this code example, make sure that the board is upgraded to KitProg3. The tool and instructions are available in the Firmware Loader GitHub repository. If you do not upgrade, you will see an error such as "unable to find CMSIS-DAP device" or "KitProg firmware is out of date".

## Software setup

See the [ModusToolbox&trade; tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package.

Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://teratermproject.github.io/index-en.html).


## Using the code example

### Create the project

The ModusToolbox&trade; tools package provides the Project Creator as both a GUI tool and a command line tool.

<details><summary><b>Use Project Creator GUI</b></summary>

1. Open the Project Creator GUI tool.

   There are several ways to do this, including launching it from the dashboard or from inside the Eclipse IDE. For more details, see the [Project Creator user guide](https://www.infineon.com/ModusToolboxProjectCreator) (locally available at *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/docs/project-creator.pdf*).

2. On the **Choose Board Support Package (BSP)** page, select a kit supported by this code example. See [Supported kits](#supported-kits-make-variable-target).

   > **Note:** To use this code example for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. On the **Select Application** page:

   a. Select the **Applications(s) Root Path** and the **Target IDE**.

   > **Note:** Depending on how you open the Project Creator tool, these fields may be pre-selected for you.

   b.	Select this code example from the list by enabling its check box.

   > **Note:** You can narrow the list of displayed examples by typing in the filter box.

   c. (Optional) Change the suggested **New Application Name** and **New BSP Name**.

   d. Click **Create** to complete the application creation process.

</details>

<details><summary><b>Use Project Creator CLI</b></summary>

The 'project-creator-cli' tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the 'project-creator-cli' tool. On Windows, use the command-line 'modus-shell' program provided in the ModusToolbox&trade; installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; tools. You can access it by typing "modus-shell" in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

The following example clones the "[Bluetooth&reg; LE CTS server](https://github.com/Infineon/mtb-example-btstack-freertos-cts-server)" application with the desired name "WPSenrollee" configured for the *CY8CKIT-062-WIFI-BT* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CY8CKIT-062-WIFI-BT --app-id mtb-example-btstack-freertos-cts-server --user-app-name CTSserver --target-dir "C:/mtb_projects"
   ```

The 'project-creator-cli' tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the <id> field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the <id> field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

> **Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at {ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf).

</details>



### Open the project

After the project has been created, you can open it in your preferred development environment.


<details><summary><b>Eclipse IDE</b></summary>

If you opened the Project Creator tool from the included Eclipse IDE, the project will open in Eclipse automatically.

For more details, see the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_ide_user_guide.pdf*).

</details>


<details><summary><b>Visual Studio (VS) Code</b></summary>

Launch VS Code manually, and then open the generated *{project-name}.code-workspace* file located in the project directory.

For more details, see the [Visual Studio Code for ModusToolbox&trade; user guide](https://www.infineon.com/MTBVSCodeUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_vscode_user_guide.pdf*).

</details>


<details><summary><b>Keil µVision</b></summary>

Double-click the generated *{project-name}.cprj* file to launch the Keil µVision IDE.

For more details, see the [Keil µVision for ModusToolbox&trade; user guide](https://www.infineon.com/MTBuVisionUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_uvision_user_guide.pdf*).

</details>


<details><summary><b>IAR Embedded Workbench</b></summary>

Open IAR Embedded Workbench manually, and create a new project. Then select the generated *{project-name}.ipcf* file located in the project directory.

For more details, see the [IAR Embedded Workbench for ModusToolbox&trade; user guide](https://www.infineon.com/MTBIARUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_iar_user_guide.pdf*).

</details>

<details><summary><b>Command line</b></summary>

If you prefer to use the CLI, open the appropriate terminal, and navigate to the project directory. On Windows, use the command-line 'modus-shell' program; on Linux and macOS, you can use any terminal application. From there, you can run various `make` commands.

For more details, see the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

If using a PSoC&trade; 64 "Secure" MCU kit (like CY8CKIT-064B0S2-4343W), the PSoC&trade; 64 device must be provisioned with keys and policies before being programmed. Follow the instructions in the ["Secure Boot" SDK user guide](https://www.infineon.com/dgdlac/Infineon-PSoC_64_Secure_MCU_Secure_Boot_SDK_User_Guide-Software-v07_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0f8c361a7666) to provision the device. If the kit is already provisioned, copy-paste the keys and policy folder to the application folder.

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud.

3. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>


   <details><summary><b>In other IDEs</b></summary>

   Follow the instructions in your preferred IDE.
   </details>


   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain is specified in the application's Makefile but you can override this value manually:
      ```
      make program TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TOOLCHAIN=GCC_ARM
      ```
   </details>

4. After programming, the application starts automatically. Observe the messages on the UART terminal, and wait for the device to initialize the required components. Use the KitProg3 COM port to view the Bluetooth&reg; stack and application trace messages in the terminal window as shown in Figure 1.

   **Figure 1. Terminal output when the device is programmed with the CE**
   ![](images/terminal_output1.png)

5. Use another supported PSoC&trade; 6 kit and program it with the [Bluetooth&reg; LE CTS Client](https://github.com/Infineon/mtb-example-btstack-freertos-cts-client) code example.

6. Once both the kits are powered, press the user button (SW2) on the client device to start the advertisement and then press button (SW2) on the server to start the scanning. The application trace messages appear in the terminal window, as shown in Figure 2.

   **Figure 2. Terminal output - Scanning and connecting**

   ![](images/terminal_output2.png)

7. After establishing the connection between the client and server, press the button(SW2) on the client to enable notifications. The server device will start sending notifications with the Current Time characteristic values derived from the device real-time clock. The date and time sent as notification are printed on the serial terminal.

   **Figure 3. Terminal output - Sending notification**

   ![](images/terminal_output3.png)

## Debugging


You can debug the example to step through the code.


<details><summary><b>In Eclipse IDE</b></summary>

Use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide).

<mark> Add the below Note for relevant CEs only, like PSoC 6 MCU based. Remove this note for others.

> **Note:** **(Only while debugging)** On the CM4 CPU, some code in `main()` may execute before the debugger halts at the beginning of `main()`. This means that some code executes twice – once before the debugger stops execution, and again after the debugger resets the program counter to the beginning of `main()`. See [KBA231071](https://community.infineon.com/docs/DOC-21143) to learn about this and for the workaround.

</details>


<details><summary><b>In other IDEs</b></summary>

Follow the instructions in your preferred IDE.
</details>



## Design and implementation

The [Bluetooth&reg; LE CTS Server](https://github.com/Infineon/mtb-example-btstack-freertos-cts-server) code example configures the device as a Bluetooth&reg; LE GAP Central - GATT Server device. Use this application with the [Bluetooth&reg; CTS Client](https://github.com/Infineon/mtb-example-btstack-freertos-cts-client), which is a GAP Peripheral - GATT Client device.

This code example showcases Current Time Service-based time profile. In this CE, the mandatory Current time characteristic with Read and notify properties are implemented. The Time profile defines two roles: Time server (GATT Server) and Time client (GATT Client).

**Figure 4. Time profile role and service relationships**

![](images/time_profile_roles.png)

The entry point of the application is `int main()`, which initializes the BSP and Bluetooth&reg; stack. The application-level initializations like RTC and GATT database initialization are handled by the `ble_app_init()` function. This function starts scanning for the peripheral device by registering a callback using `wiced_bt_ble_scan()`.

This application will specifically scan for advertisement with the Peripheral device name `CTS Client` and establish a LE GATT connection. All the GATT events are handled in `ble_app_gatt_event_handler()`. During Read or Notify GATT operations, the fields of the Current Time characteristic are set to values derived from the local date and time and sent as GATT Read response or as notification to the peripheral device. The same data is printed on the serial terminal.

The RTC provides time and date information – second, minute, hour, day of the week, date, month, and year using the RTC driver API. The time and date information are updated every second with automatic leap year compensation performed by the RTC hardware block. The RTC initialization is also done in `ble_app_init()`.

The application uses a UART resource from the Hardware Abstraction Layer (HAL) to print debug messages on a UART terminal emulator. The UART resource initialization and retargeting of the standard I/O to the UART port is done using the retarget-io library.

### Power measurement: Implementation of low power for AIROC&trade; Bluetooth&reg; LE**

This examples enables you to measure power in three different AIROC&trade; Bluetooth&reg; LE states: Standby, Scanning, and Connected. Do the following to enter each state and measure the power consumption for different kits.

1. **Standby state:** After programming the device, AIROC&trade; Bluetooth&reg; LE is initialized and stays in standby state. On the terminal, check for the message 'Bluetooth&reg; stack initialization successful'; you can measure power for standby state.

2. **Scanning state:** Press the user button (SW2) on your kit to start scanning. Note that the kit with CTS client CE must be advertising when scanning is started. When it discovers the peer device, it sends a connection request. A high duty scan will be performed initially for 30 seconds. Then the device switches to low duty scanning without timeout. High duty scanning of 30 seconds is chosen for faster discovery. If required, this configuration can be changed using the 'bt-configurator' tool that comes with ModusToolbox&trade; installation.

3. **Connected state:** Once the scanner finds the advertiser, a connection is established. The terminal displays the message 'Connected : BDA xx:xx:xx:xx:xx:xx'.

### Current measuring points for kits

#### **CY8CKIT-062S2-43012, CYW9P62S1-43438EVB-01, CYW9P62S1-43012EVB-01 and CY8CKIT-064B0S2-4343W**

*For PSoC&trade; 6 MCU:*

1. Remove J25 to eliminate leakage currents across potentiometer R1.

2. Measure the current at J15 across VTARG and P6_VDD.

*For CYW43xxx:*

- Measure the current at VBAT across VBAT and VCC_VBAT at J8.

#### **CY8CEVAL-062S2-LAI-4373M2**

*For PSoC&trade; 6 MCU:*

1. Remove J21 to eliminate leakage currents across potentiometer R1.

2. Measure the current at J15 across VTARG and P6_VDD.

*For CYW4373E:*

- Measure the current at VBAT across VBAT and VCC_VBAT at J11.

#### **CY8CPROTO-062-4343W**

*For PSoC&trade; 6 MCU:*

1. Remove R65 on the right of the board close to the USB connector of the PSoC&trade; 6 MCU device.

2. Connect an ammeter between VTARG (J2.32) and P6_VDD (J2.24).

3. Remove R24 at the back of the board, below J1.9, to eliminate the leakage current.

   R24 is the pull-up resistor attached to the WL_HOST_WAKE pin P0_4, which leaks approximately 330 µA because P0_4 is driven LOW when there is no network activity. In total, the PSoC&trade; 6 MCU deep sleep current is approximately 350 µA.

*For CYW4343W:*

1. Measure the current at VBAT1 and VBAT2 supplies used for powering CYW4343W. VBAT1 and VABT2 are shorted to each other.

2. Remove R87 on the back of the board towards the right and above J2.33.
3. Connect an ammeter between the pads of R87 to measure the current.

#### **CY8CKIT-062-WIFI-BT**

*For PSoC&trade; 6 MCU:*

- Measure the current  by connecting an ammeter to the PWR MON jumper J8.

*For CYW4343W:*

- Measure the current at WL_VBAT (used for powering CYW4343W) by removing L3 along the right edge of the board close to the CYW4343W module, and connecting an ammeter between the pads of L3.

#### **CY8CPROTO-062S3-4343W**

*For PSoC&trade; 6 MCU:*

- Measure the current by removing R59 and connecting an ammeter across VTARG (J2.32) and P6_VDD (J2.31)

*For CYW4343W:*

- Measure the current by removing R55 and connecting an ammeter between the resistor pads (VCC_3V6 and VBAT_WL).

Table 1 captures the current numbers measured using this CE for two BSPs. The kits have different connectivity devices - CYW43012 and CYW43438. The measurement is not performed in a radio isolated environment. The current consumption by the PSoC&trade; 6 device is also measured; it was identical across all Bluetooth&reg; states. The following are the average current values:

1. For PSoC&trade; 6 device in CY8CKIT-062S2-43012: 27 µA

2. For PSoC&trade; 6 device in CYW9P62S1-43438EVB-01: 620 µA

3. For PSoC&trade; 6 device in CY8CEVAL-062S2-LAI-4373M2: 23 µA

**Table 1. Current consumption**

 |Bluetooth&reg; state|Setting |CY8CKIT-062S2-43012|CYW9P62S1-43438EVB-01|CY8CEVAL-062S2-LAI-4373M2|
 | :-------      | :------------      | :------------   | :------------ | :----------- |
 |Standby state      |Stack initialized      |3.65 mA  |4.21 mA   |10.71 mA  |
 |High duty scanning |Scan interval: 120 ms  |4.42 mA  |5.67 mA   |7.9 mA    |
 |Low duty scanning  |Scan interval: 2560 ms |86.23 uA |111.78 uA |535.485 uA|


### Resources and settings

This section explains the ModusToolbox&trade; resources and their configuration as used in this code example. Note that all the configuration explained in this section has already been done in the code example.

- **Device Configurator:** ModusToolbox&trade; stores the configuration settings of the application in the *design.modus* file. This file is used by the Device Configurator, which generates the configuration firmware. This firmware is stored in the application’s *GeneratedSource* folder. By default, all applications in a workspace share the same *design.modus* file - i.e., they share the same pin configuration. Each BSP has a default *design.modus* file in the *mtb_shared\TARGET_\<bsp name\>\\<version\>\COMPONENT_BSP_DESIGN_MODUS* directory.

   It is not recommended to modify the configuration of a standard BSP directly. To modify the configuration for a single application or to create a custom BSP refer to the [ModusToolbox&trade; user guide](https://www.infineon.com/dModusToolboxUserGuide). This example uses the default configuration.

   For detailed information on how to use the Device Configurator, see the [Device Configurator guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Device_Configurator_Guide_4-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99ab297631cb).

- **Bluetooth&reg; Configurator:** The Bluetooth&reg; peripheral has an additional configurator called the “Bluetooth&reg; Configurator” that is used to generate the AIROC&trade; Bluetooth&reg; LE GATT database and various Bluetooth&reg; settings for the application. These settings are stored in the file named *design.cybt*.

   Note that unlike the Device Configurator, the Bluetooth&reg; Configurator settings and files are local to each respective application.

   For detailed information on how to use the Bluetooth&reg; Configurator, see the [Bluetooth&reg; Configurator guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Bluetooth_Configurator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99aaf5b231be).

**Note:** For PSoC&trade; 6 Bluetooth&reg; LE-based BSPs(CY8CKIT-062-BLE, CY8CPROTO-063-BLE, CYBLE-416045-EVAL) with support for AIROC&trade; BTSTACK, if you want to use the bt-configurator tool, select the *AIROC&trade; BTSTACK with Bluetooth&reg; LE only (CYW20829, PSoC&trade; 6 with CYW43xxx Connectivity device)* option from the dropdown to select the device. Do not use the *PSoC&trade; Bluetooth&reg; LE Legacy Stack (PSoC&trade; 6-BLE)* option because it is not compatible with AIROC&trade; BTSTACK.

**Table 2. Application resources**

 Resource  |  Alias/object     |    Purpose
 :-------- | :-------------    | :------------
 UART (HAL)|cy_retarget_io_uart_obj| UART HAL object used by Retarget-IO for Debug UART port
 GPIO (HAL)    | CYBSP_USER_BTN         | Start advertisement or enable/disable notification


## Related resources

Resources  | Links
-----------|----------------------------------
Application notes  | [AN228571](https://www.infineon.com/AN228571) – Getting started with PSoC&trade; 6 MCU on ModusToolbox&trade; software <br>  [AN215656](https://www.infineon.com/AN215656) – PSoC&trade; 6 MCU: Dual-CPU system design
Code examples  | [Using ModusToolbox&trade;](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation | [PSoC&trade; 6 MCU datasheets](https://www.infineon.com/cms/en/search.html#!view=downloads&term=psoc6&doc_group=Data%20Sheet) <br> [PSoC&trade; 6 technical reference manuals](https://www.infineon.com/cms/en/search.html#!view=downloads&term=psoc6&doc_group=Additional%20Technical%20Information)<br>[AIROC&trade; CYW20829 Bluetooth&reg; LE SoC](https://www.infineon.com/cms/en/product/promopages/airoc20829)
Development kits | Select your kits from the [Evaluation board finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board).
Libraries on GitHub  | [mtb-pdl-cat1](https://github.com/Infineon/mtb-pdl-cat1) – PSoC&trade; 6 Peripheral Driver Library (PDL)  <br> [mtb-hal-cat1](https://github.com/Infineon/mtb-hal-cat1) – Hardware Abstraction Layer (HAL) library <br> [retarget-io](https://github.com/Infineon/retarget-io) – Utility library to retarget STDIO messages to a UART port
Middleware on GitHub  | [capsense](https://github.com/Infineon/capsense) – CAPSENSE&trade; library and documents <br> [psoc6-middleware](https://github.com/Infineon/modustoolbox-software#psoc-6-middleware-libraries) – Links to all PSoC&trade; 6 MCU middleware
Tools  | [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use libraries and tools enabling rapid development with Infineon MCUs for applications ranging from wireless and cloud-connected systems, edge AI/ML, embedded sense and control, to wired USB connectivity using PSoC&trade; Industrial/IoT MCUs, AIROC&trade; Wi-Fi and Bluetooth&reg; connectivity devices, XMC&trade; Industrial MCUs, and EZ-USB&trade;/EZ-PD&trade; wired connectivity controllers. ModusToolbox&trade; incorporates a comprehensive set of BSPs, HAL, libraries, configuration tools, and provides support for industry-standard IDEs to fast-track your embedded application development.

<br>



## Other resources

Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.

For PSoC&trade; 6 MCU devices, see [How to design with PSoC&trade; 6 MCU – KBA223067](https://community.infineon.com/docs/DOC-14644) in the Infineon Developer Community.

## Document history

Document title: *CE230302* – *Bluetooth&reg; LE CTS Server*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example    
 2.0.0   | Major update to support ModusToolbox&trade; software v2.3.1,<br>This version is not backward compatible with ModusToolbox&trade; software v2.2 or older versions, <br>Added support for new kits </br> Enabled for CYW43xxx Low power and addition of current numbers 
 3.0.0   | Added support for 43439 kit <br> Updated BSP to 3.0.0
 4.0.0   | Updated to support ModusToolbox&trade; software v3.0 and BSPs v4.x
 4.1.0   | Added support for CYW920829M2EVB-01, CY8CKIT-062-BLE, CY8CPROTO-063-BLE, and CYBLE-416045-EVAL
 4.2.0   | Added support for CY8CEVAL-062S2-LAI-43439M2 and CY8CPROTO-062S2-43439
 4.3.0   | Removed CYW920829M2EVB-01 from supported kits <br> Added support for CYW920829M2EVK-02
 4.4.0   | Added support for CY8CEVAL-062S2-MUR-4373EM2 and CY8CEVAL-062S2-MUR-4373M2 
 4.5.0   | Updated error handling and gatt_status
 4.6.0   | Update the README with new template and added support for CYW989829M2EVB-01 and CY8CEVAL-062S2-CYW43022CUB
 4.7.0   | Added support for BSP CY8CKIT-062S2-AI
------

All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.


---------------------------------------------------------

© Cypress Semiconductor Corporation, 2020-2024. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress's patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress's published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, ModusToolbox, PSoC, CAPSENSE, EZ-USB, F-RAM, and TRAVEO are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit www.infineon.com. Other names and brands may be claimed as property of their respective owners.
