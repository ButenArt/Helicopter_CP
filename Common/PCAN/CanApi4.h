#pragma once

///////////////////////////////////////////////////////////////////////////////
//  CanApi4.h: Definition of the CAN-API
//
//  Version 4.3.1
//
//  Principle:
//  ~~~~~~~~~~
//  The driver supports multiple clients (= Windows applications that
//  communicate with CAN buses), and multiple CAN hardware implemented
//  with SJA1000 CAN controllers.
//  A cardinal point is the idea of the "net": it describes a CAN bus that
//  is extended virtually into the PC. Multiple clients can be connected
//  to one net, which itself can have an interface to a physical CAN bus
//  via an appropriate CAN adapter.
//  A net definition determines, aside from the Bit rate, an amount
//  of CAN messages to process.
//
//  Clients that are specialized on some kind of CAN bus (e.g. stepper
//  motor control, car radio panel, etc.), should not offer any hardware
//  selection, but directly address a fixed net (e.g. 'Lab-Net').
//  The connection net - hardware can then be accomplished by a separate
//  configuration tool (the settings depend on the respective PC and its
//  CAN hardware).
//
//  If necessary, CAN nodes connected to an external CAN bus can 
//  be simulated by clients on the same net. In this case there is no
//  CAN hardware required, the complete bus can be simulated within the
//  PC. The net can then be defined as an 'Internal Net'.
//
//  Samples for possible net configurations:
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  (can all be realized at the same time):
//                                                   external
//                                    ,------------< CAN bus 'A'
//  ,--------. ,--------.       ,-----+----.
//  |Client A| |Client B|       |Hardware 1|
//  `---+----' `----+---'       `-----+----'
//      `-----------+-----------------'
//               N e t  I                           external
//                                    ,------------< CAN bus 'B'
//  ,--------. ,--------.       ,-----+----.
//  |Client C| |Client D|       |Hardware 2|
//  `---+--+-' `----+---'       `-----+----'
//      |  `--------+-----------------'              external
//      |        N e t  II            ,------------< CAN bus 'C'
//      |      ,--------.       ,-----+----.
//      |      |Client E|       |Hardware 3|
//      |      `----+---'       `-----+----'
//      `-----------+-----------------'             'Gateway'
//               N e t  III
//   ,--------. ,--------. ,--------.
//   |Client F| |Client G| |Client H|
//   `---+----' `---+----' `---+----'               'Internal net'
//       `----------+----------'
//               N e t  IV
//
//  Features:
//  ~~~~~~~~~
//   - 1 client can be connected to multiple nets
//   - 1 net supplies multiple clients
//   - 1 hardware can be used by 1 net at the same time
//   - each net can be assigned to 1 hardware or no hardware at all
//   - if a client sends a message on the net, the message will be routed
//     to all other clients and over a connected hardware to the physical
//     bus
//   - if a message is received from a hardware, it will be routed to all
//     clients which are connected to the hardware via a net. Each client
//     only receives the messages which pass its acceptance filter
//   - CAN hardware can be configured via a Windows Control Panel application,
//     nets can be configured with a separate tool.
//     Multiple nets can be defined for each hardware, but only one can be
//     active at the same time.
//   - clients connect to a net via the name of the net
//   - each hardware has its own transmit queue to buffer outgoing messages
//   - each client has a receive queue to buffer received messages
//   - each client has a transmit queue, which holds outgoing messages until
//     their scheduled real send time. Is the send time reached they will
//     be written into the transmit queue of the hardware.
//   - client: 'client handle'. This number is used by the driver to
//             identify and manage a client
//   - hw:     'hardware handle'. This number is used by the driver to
//             identify and manage a hardware
//   - net:    'net handle'. This number is used by the driver to
//             identify and manage a net
//   - all handles are 1-based. 0 = illegal handle
//   - used hardware and nets are defined in the Registry.
//     When a PCAN driver is loaded, it reads the configuration and
//     initializes all hardware and nets.
//
//  Registry Keys:
//  10/8.1/7:
//      HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Pcan_usb
//      HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Pcan_pci
//      HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Pcan_pccard
//      HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Pcan_virtual
//      HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Pcan_lan
//
//  Values (as strings):
//      Net<NetHandle>=<Name>,<HwHandle>,<BTR0BTR1>
//
//  Example:
//      Hardware1=1,0x300,15
//      Net7=TestNet,1,0x001C
//
//   - the API functions are divided into 3 groups:
//     1) Control-API: control of the driver through configuration tools
//     2) Client-API: reading and writing of messages through applications
//     3) Info-API: helper functions
//
//
//  Control-API
//  ~~~~~~~~~~~
//  CAN_RegisterNet(device, net, name, hw, bitrate)
//                  Creates a new net, makes an assignment net - hardware.
//                  Net can be accessed in future via net handle.
//  CAN_RemoveNet(device, net)
//                  Deletes a net, the net handle gets invalid.
//  CAN_ResetClient(device, client)
//                  Resets the receive and transmit queues of a client.
//
//  Client-API
//  ~~~~~~~~~~
//
//  Hardware control:
//  ~~~~~~~~~~~~~~~~~
//  CAN_GetHardwareStatus(device, hw)
//                  Gets the current state of a hardware.
//  CAN_ResetHardware(device, hw)
//                  Resets the CAN controller, resets the transmit queue of a
//                  hardware. Affects other clients on the same net.
//
//  Read/Write:
//  ~~~~~~~~~~~
//  CAN_Write(device, client, net, &buffer, bytesToWrite, &bytesWritten)
//                  Writes a stream of messages to a net.
//                  The messages will be sent to the linked hardware and to all
//                  clients that have established a connection to the net with
//                  'CAN_ConnectToNet()'.
//  CAN_Read(device, client, &buffer, bytesToRead, &bytesRead)
//                  Reads messages from the receive queue.
//
//  Registration and connection of clients:
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  CAN_RegisterClient(device, name, wnd, &client)
//                  Registers a client at the driver,
//                  gets a client handle and initializes the receive queue
//                  (one call per client).
//  CAN_ConnectToNet(device, client, netName, &net)
//                  Connects a client to a net
//                  (one call per client and net).
//  CAN_ConnectToHardware(device, client, params, &net)
//                  Connects a client to a hardware, automatically creates a
//                  temporary net (one call per client and hardware).
//  CAN_RegisterMessages(device, client, net, IDfrom, IDto, extended)
//                  A client indicates that it wants to receive messages from
//                  a net. The messages in the range IDfrom to IDto will be received.
//                  There is only ONE filter for 11-bit and 29-bit messages.
//                  The 11-bit messages will be registered as if the ID was
//                  built with the bits 28..18.
//                  Example: registration of 11-bit ID 0x400 means that the
//                  29-bit ID 0x10000000 will be also received.
//                  Every call of this function might open the receive filter
//                  of the CAN Controller. If this happens, the CAN Controller
//                  will perform a hardware reset cycle.
//                  If a client wants answer Remote Request messages, these
//                  messages must be also registered with this function.
//                  It is not guaranteed that a client only receives those
//                  messages that were registered using this function. This
//                  depends on the used CAN Controller (usually SJA1000).
//  CAN_SetClientFilter(device, client, net, filterIndex, filterMode,
//                      extended, accCode, accMask)
//                  Sets the client message filter directly (SJA1000-type)
//                  (alternative method to CAN_RegisterMessages())
//  CAN_ResetClientFilter(device, client)
//                  Resets the filter of a client.
//  CAN_DisconnectFromNet(device, client, net)
//                  Disconnects a client from a net.
//  CAN_RemoveClient(client)
//                  Removes a client from the driver. Frees resources.
//
//
//  Info-API
//  ~~~~~~~~
//  CAN_GetSystemTime(device, &time)
//                  Accesses the function 'Get_System_Time()' of the VMM:
//                  Returns time in microseconds since Windows start.
//  CAN_GetParam(device, &buffer)
//                  Gets the value of a driver/hardware/net/client parameter.
//  CAN_SetParam(device, &buffer)
//                  Sets a driver/hardware/net/client parameter.
//  CAN_GetErrText(error, &textBuff, buffSize)
//                  Transforms error flags into text.
//  CAN_BitrateToBitsPerSecond(&bitrate_record, &nominal_bps, &data_bps)
//                  Calculates true bits-per-second values from bit rate parameter
//                  records.
//  CAN_GetAvailableHardware(deviceFilter, hardwareBuff, maxHardware, &hardwareCount)
//                  Returns information about the CAN hardware channels currently
//                  available on the computer.
//
//
//  Samples for API usage:
//  ~~~~~~~~~~~~~~~~~~~~~~
//  a) Initialization of hardware and nets at Windows start:
//     in 'Driver - OnDeviceInit()':
//          // controlled through Registry:
//                  ...
//          RegisterHardware();       // Initialize every found hardware
//          RegisterHardware();
//                  ...
//          RegisterNet();            // Load net definitions
//          RegisterNet();
//                  ...
//
//  b) Configuration tool:
//          LoadConfigFromRegistry();
//          EditConfig();             // User sets up a configuration
//          SaveConfigToRegistry();
//          // Controlled by the configuration
//          CAN_RegisterNet(); SaveNetToRegistry();
//          CAN_RegisterNet(); SaveNetToRegistry();
//                  ....
//          // New configuration is now active, even after Windows is restarted.
//          // All previously connected clients are now dead.
//
//  c) Client
//          can_device_t mydevice = pcan_usb;
//          CAN_RegisterClient(mydevice, ..., &myclient);    // Just once
//          CAN_ConnectToNet(mydevice, ..., &mynet)
//          // CAN_ConnectToNet();    // Perhaps multiple, e.g. if gateway
//          if (own_bitrate)
//          {
//              can_any_param_t param;
//              param.base.size = sizeof(param);
//              param.base.type = CAN_PARAM_NETHW_ACTIVE;
//              param.base.objclass = CAN_PARAM_OBJCLASS_NET;
//              param.base.objhandle = mynet;
//              if (CAN_GetParam(mydevice, &param) == CAN_ERR_OK)
//              {
//                  HCANHW hw = (HCANHW) param.uint32.value;
//                  if (hw > 0)  // check if net is associated with a hardware
//                  {
//                      param.base.type = CAN_PARAM_BITRATE;
//                      param.base.objclass = CAN_PARAM_OBJCLASS_HARDWARE;
//                      param.base.objhandle = hw;
//                      param.uint32.value = mybitrate;
//                      CAN_SetParam(mydevice, &param);
//                  }
//              }
//          }
//
//          CAN_RegisterMessages(mydevice, myclient, mynet, ...);    // Configure reception filter
//
//          while (active)
//          {
//              UINT8 buffer[1000];
//              can_recordheader_t *read_pointer;
//              UINT32 bytesRead = 0;
//              UINT32 bytesProcessed = 0;
//              if (CAN_Read(mydevice, myclient, &buffer, sizeof(buffer), &bytesRead) == CAN_ERR_OK)
//              {
//                  // Something has been received
//                  while (bytesProcessed < bytesRead)
//                  {
//                      read_pointer = (can_recordheader_t*)(buffer + bytesProcessed);
//                      switch(read_pointer->type)
//                      {
//                      case CAN_RECORDTYPE_msg:
//                          ProcessMessage((can_msg_t*) read_pointer);
//                          break;
//                      case CAN_RECORDTYPE_msg_rtr:
//                          ProcessRTR((can_msg_rtr_t*) read_pointer);
//                          break;
//                      case CAN_RECORDTYPE_msg_fd:
//                          ProcessMessageFD((can_msg_fd_t*) read_pointer);
//                          break;
//                      case ...
//                      }
//                      bytesProcessed += read_pointer->size;
//                  }
//              }
//
//              if (something_to_write)
//                  CAN_Write();
//              if (something_exceptional)
//              {
//                  CAN_ResetHardware();
//                  CAN_ResetClient();
//              }
//          }
//
//          CAN_RemoveClient();       // Just once, free resources
//
//
//  Provided constants:
//  CAN_BITRATE_...   Default Bit rate codes for non-CAN-FD hardware
//  CAN_PARAM_ ...    Parameter codes
//
//  All functions return a combination of error states CAN_ERR_xxx.
//
//
//  Authors:  Hoppe, Wolf
//  Language: C, C++
//
//  --------------------------------------------------------------------
//  Copyright (C) 2020 PEAK-System Technik GmbH, Darmstadt, Germany
//  All rights reserved.

#ifdef __cplusplus
extern "C" {
#endif

// Constants definitions

// Maximum values for CAN identifiers
#define CAN_MAX_STANDARD_ID     0x7ff
#define CAN_MAX_EXTENDED_ID     0x1fffffff

// Bit rate codes = BTR0/BTR1 register values for non-CAN-FD hardware
#define CAN_BITRATE_1M     0x0014  //     1 MBit/s
#define CAN_BITRATE_800K   0x0016  //   800 kBit/s
#define CAN_BITRATE_500K   0x001C  //   500 kBit/s
#define CAN_BITRATE_250K   0x011C  //   250 kBit/s
#define CAN_BITRATE_125K   0x031C  //   125 kBit/s
#define CAN_BITRATE_100K   0x432F  //   100 kBit/s
#define CAN_BITRATE_95K    0xC34E  // 95.23 kBit/s
#define CAN_BITRATE_83K    0x852B  // 83.33 kBit/s
#define CAN_BITRATE_50K    0x472F  //    50 kBit/s
#define CAN_BITRATE_47K    0x1414  //  47.6 kBit/s
#define CAN_BITRATE_33K    0x8B2F  // 33.33 kBit/s
#define CAN_BITRATE_20K    0x532F  //    20 kBit/s
#define CAN_BITRATE_10K    0x672F  //    10 kBit/s
#define CAN_BITRATE_5K     0x7F7F  //     5 kBit/s

// Error Codes
#define CAN_ERR_OK             0x0000  // No error
#define CAN_ERR_XMTFULL        0x0001  // Transmit buffer in CAN controller is full
#define CAN_ERR_OVERRUN        0x0002  // CAN controller was read too late
#define CAN_ERR_BUSWARNING     0x0008  // Bus error: an error counter reached the 'warning' limit  
#define CAN_ERR_BUSPASSIVE     0x40000 // Bus error: CAN controller is in Error Passive state
#define CAN_ERR_BUSOFF         0x0010  // Bus error: CAN controller is in Bus-off state
#define CAN_ERR_QRCVEMPTY      0x0020  // Receive queue is empty
#define CAN_ERR_QOVERRUN       0x0040  // Receive queue was read too late
#define CAN_ERR_QXMTFULL       0x0080  // Transmit queue ist full
#define CAN_ERR_REGTEST        0x0100  // Test of the CAN controller hardware registers failed (no hardware found)
#define CAN_ERR_NODRIVER       0x0200  // Driver not loaded
#define CAN_ERRMASK_ILLHANDLE  0x1C00  // Mask for all handle errors
#define CAN_ERR_HWINUSE        0x0400  // Hardware already in use by a net
#define CAN_ERR_NETINUSE       0x0800  // A client is already connected to the net
#define CAN_ERR_ILLHW          0x1400  // Hardware is invalid
#define CAN_ERR_ILLNET         0x1800  // Net is invalid (handle/name)
#define CAN_ERR_ILLCLIENT      0x1C00  // Client is invalid
#define CAN_ERR_RESOURCE       0x2000  // Resource (queue, client, timer) cannot be created
#define CAN_ERR_ILLPARAMTYPE   0x4000  // Invalid parameter
#define CAN_ERR_ILLPARAMVAL    0x8000  // Invalid parameter value
#define CAN_ERR_UNKNOWN        0x10000 // Unknown error
#define CAN_ERR_ILLFUNCTION    0x20000 // CAN-API function not supported
#define CAN_ERR_ILLMODE        0x80000 // Object in wrong state for attempted operation
#define CAN_ERR_SYSTEMERROR_MASK  0x80000000 // Mask for Windows system error codes

#define CAN_ERR_ANYBUSERR (CAN_ERR_BUSWARNING | CAN_ERR_BUSPASSIVE | CAN_ERR_BUSOFF)

// Parameter codes for SetParam|GetParam

// Hardware: index number of the driver-internal hardware sub-type (PCAN-USB, PCAN-USB Pro, ...) (uint32)
#define CAN_PARAM_HWDRIVERNR  2 // 0x02

// Name of the hardware/driver/net/client (string255)
#define CAN_PARAM_NAME  3 // 0x03

// Hardware: I/O address of the hardware (uint32)
#define CAN_PARAM_HWPORT  4 // 0x04

// Hardware interrupt (uint32)
#define CAN_PARAM_HWINT  5 // 0x05

// Hardware: the net that is connected to the hardware (uint32)
#define CAN_PARAM_HWNET  6 // 0x06

// Hardware/net: Bit rate, as BTR0/BTR1 code (uint32)
#define CAN_PARAM_BITRATE  7 // 0x07

// Hardware: CAN controller operation mode (uint32)
// 0 = controller is in Reset mode, 1 = Operation mode 
#define CAN_PARAM_ACTIVE  10 // 0x0a

// Hardware/client: unsent messages in transmit queue (uint32)
#define CAN_PARAM_XMTQUEUEFILL  11 // 0x0b

// Hardware/client: unread messages in receive queue (uint32)
#define CAN_PARAM_RCVQUEUEFILL  12 // 0x0c

// Net: hardware handle associated with net (uint32)
#define CAN_PARAM_NETHW         19 // 0x13  deprecated
#define CAN_PARAM_NETHW_ACTIVE  19 // 0x13

// Net: Flag: clients[i] <> 0: client 'i' belongs to net (string255)
#define CAN_PARAM_NETCLIENTS  20 // 0x14

// Client: window handle of client (uint32)
#define CAN_PARAM_HWND  21 // 0x15

// Client: Flag: nets[i] <> 0: net 'i' belongs to client (string255)
#define CAN_PARAM_CLNETS  22 // 0x16

// Hardware/client: transmit queue size (uint32)
#define CAN_PARAM_XMTQUEUESIZE  23 // 0x17

// Hardware/client: receive queue size (uint32)
#define CAN_PARAM_RCVQUEUESIZE  24 // 0x18

// Client: handle of Receive Event (uint32)
#define CAN_PARAM_ONRCV_EVENT_HANDLE  26 // 0x1a

// Client: trigger mode of Receive Events (uint32)
// 0 = set (default), 1 = pulse
#define CAN_PARAM_ONRCV_EVENT_PULSE  27 // 0x1b

// Client: enables/disables self-receive (uint32)
// 0 = self-receive disabled (default)
// 1 = client receives all of its own transmitted messages
#define CAN_PARAM_SELF_RECEIVE  28 // 0x1c

// Net: Delayed Message Distribution (uint32)
// 0 = transmits the messages to the other clients while writing into the
//     hardware queue (default)
// 1 = transmits the messages to the other clients only when hardware has
//     successfully transmitted the message on the bus
#define CAN_PARAM_DELAYED_MESSAGE_DISTRIBUTION  29 // 0x1d

// Hardware (ExpressCard/ExpressCard 34): Unique reseller/distributor
// code for OEM hardware (uint32)
#define CAN_PARAM_HW_OEM_ID  30 // 0x1e

// Hardware: location info text that describes the "position" of the
// hardware used (string255)
// Example: "I/O addr 0x378", "PCI bus 0, slot 7, controller 1"
// Can be specified in the registry or will be created automatically
#define CAN_PARAM_LOCATION_INFO  31 // 0x1f

// Hardware: PCI bus to which the hardware is connected (uint32)
#define CAN_PARAM_HWBUS  32 // 0x20

// Hardware: PCI slot to which the hardware is connected (uint32)
#define CAN_PARAM_HWDEVICE  33 // 0x21

// Hardware: PCI function of card (uint32)
#define CAN_PARAM_HWFUNCTION  34 // 0x22

// Hardware/net: the number of the CAN Controller (uint32)
#define CAN_PARAM_HWCONTROLLER  35 // 0x23

// Hardware: measured bus load values of PCAN-USB Pro and FD-capable PCAN hardware (uint32)
#define CAN_PARAM_BUSLOAD  38 // 0x26

// Hardware/net: enable/disable Listen-only mode (uint32)
// 0 = Listen-only mode disabled (default), 1 = Listen-only mode enabled
#define CAN_PARAM_LISTEN_ONLY  49 // 0x31

// Hardware/net: Device ID (uint32)
#define CAN_PARAM_HW_DEVICENR  50 // 0x32

// Hardware: PEAK serial number (uint32)
#define CAN_PARAM_HW_SERNR  51 // 0x33

// Client/net/hardware: enable/disable Error Frames (uint32)
// 0 = Error Frames disabled (default)
// 1 = Error Frames enabled, client receives Error Frames in can_errorframe_t records
// Parameter is read-only for hardware and net
#define CAN_PARAM_RCVERRFRAMES  53 // 0x35

// Client: exact 11-bit filtering (uint32)
// 0 = client filters by code/mask (default)
// 1 = client filters exact message ranges
#define CAN_PARAM_EXACT_11BIT_FILTER  56 // 0x38

// Hardware: location info that the user can set (USB string descriptor) (string255)
#define CAN_PARAM_USER_LOCATION_INFO  57 // 0x39

// Hardware: controls the "Select" LED of PCAN hardware (uint32)
// PCAN-USB (firmware version < 8):
//   0/1: causes an LED status change (LED blinks once)
// All other USB devices:
//   0 = LED off, 1 = LED on, 2 = LED blinks slow, 3 = LED blinks fast,
//   0xFFFFFFFF = revert to default state
#define CAN_PARAM_SELECT_LED  58 // 0x3a

// Net: client handle of net master (uint32)
// 0 = no master defined (default)
#define CAN_PARAM_NET_MASTER  66 // 0x42

// Hardware: enables 5V output on CAN connector (PCAN-PC Card/ExpressCard only) (uint32)
// 0 = 5V output disabled (default), 1 = 5V output enabled
#define CAN_PARAM_BUSPOWER  79 // 0x4f

// Hardware/net: Error Warning Limit in CAN controller (uint32)
#define CAN_PARAM_ERROR_WARNING_LIMIT  83 // 0x53

// Client/hardware: Dual Filter Mode: use 1 or 2 acceptance filters (uint32)
#define CAN_PARAM_ACCFILTER_COUNT  84 // 0x54

// Hardware: patch for PCAN-USB, sets the Reset/Bus-On mode of SJA1000 (uint32)
#define CAN_PARAM_BUSON  90 // 0x5a

// Driver: load "hardware" keys from Registry (uint32)
// 0 = disabled, 1 = enabled (default)
#define CAN_PARAM_REGISTRYHARDWARELOADING  92 // 0x5c

// Driver: automatic Bus-On (uint32)
// 0 = automatic Bus-On disabled, 1 = automatic Bus-On after Bus-Off
#define CAN_PARAM_AUTOBUSON  96 // 0x60

// Hardware: enable/disable bus load measurement in hardware (PCAN-USB Pro and FD-capable
// PCAN hardware) (uint32)
// 0 = bus load measurement disabled (default), 1 = bus load measurement enabled
#define CAN_PARAM_BUSLOAD_ENABLE  106 // 0x6a

// Hardware: creation of bus errors (PCAN-USB Pro and FD-capable PCAN hardware)
// (can_param_buserrorgeneration_t)
#define CAN_PARAM_BUSERRORGENERATION  110 // 0x6e

// Client: accumulated ERR_QXMTFULL errors of a hardware, which occurred while the client was
// sending from its transmit queue (read-only) (uint32)
#define CAN_PARAM_XMTQUEUE_ERR_QXMTFULL_COUNT  113 // 0x71

// Client: enable/disable RTR frames (uint32)
// 0 = RTR frames disabled
// 1 = RTR frames enabled (default), client receives RTR frames in can_msg_rtr_t records
#define CAN_PARAM_RCVRTRFRAMES  119 // 0x77

// Client: enable/disable status frames (uint32)
// 0 = Status frames disabled
// 1 = Status frames enabled (default), client receives Status frames in can_hwstatus_t records
#define CAN_PARAM_RCVSTATUSFRAMES  120 // 0x78

// Hardware/net: indicates whether a hardware is FD-capable or whether a net is a CAN FD net
// (read-only) (uint32)
// Hardware: 0 = Hardware is not FD-capable, 1 = hardware is FD-capable
// Net: 0 = Net is a CAN 2.0B net, 1 = net is a CAN FD net
#define CAN_PARAM_IS_FD  127  // 0x7f

// Hardware: IP address of PCAN_LAN device (string255)
#define CAN_PARAM_IPADDRESS  128  // 0x80

// Hardware: number of CAN controllers per hardware (uint32)
#define CAN_PARAM_HWCONTROLLER_COUNT  129 // 0x81

// Hardware: PEAK part number (string255)
#define CAN_PARAM_PARTNO  130 // 0x82

// Hardware (PCAN_LAN): number of receive/transmit routes (uint32)
#define CAN_PARAM_LAN_RCVROUTE_COUNT  131 // 0x83
#define CAN_PARAM_LAN_XMTROUTE_COUNT  132 // 0x84

// Client: limits the number of records returned by CAN_Read() (uint32)
// Default = 0 = no limit
#define CAN_PARAM_READ_MAX_RECORDCOUNT  133 // 0x85

// Client/net: enables/disables bus load frames (uint32)
// 0 = bus load frames disabled (default)
// 1 = bus load frames enabled, client receives bus load data in can_busload_t records
// Bus load measurement must be enabled separately for the hardware (CAN_PARAM_BUSLOAD_ENABLE) 
// to receive bus load data
// Parameter is read-only for net
#define CAN_PARAM_RCVBUSLOADFRAMES  134 // 0x86

// Client: enables/disables Events (uint32)
// 0 = events disabled (default)
// 1 = events enabled, client receives events in can_event_..._t records
#define CAN_PARAM_RCVEVENTS  135 // 0x87

// Hardware/net: CAN-FD ISO/non-ISO mode for FD-capable PCAN hardware (uint32)
// 0 = old Bosch standard, no Stuff Bit counter
// 1 = new ISO mode with Stuff Bit counter (default for hardware)
// 0xFFFFFFFF (net only) = ISO mode is determined by hardware configuration (default for net)
#define CAN_PARAM_CANFD_ISOMODE  136 // Deprecated, use CAN_PARAM_FD_ISOMODE
#define CAN_PARAM_FD_ISOMODE  136 // 0x88

// Hardware (USB hardware): OEM code of hardware (uint64)
// Valid OEM code is needed for driver for password access.
// Write through to device only possible with PEAK tools.
#define CAN_PARAM_OEM_CODE  137 // 0x89

// Hardware (FD-capable USB hardware): password protection for 
// CAN_PARAM_USERDATA_SECURE device field (string255)
// Write: unlock device by sending a password, OEM code must have been set before.
// Read: one of the strings "OEM_CODE_VIRGIN", "PW_VIRGIN", "PW_FAIL", "PW_OK"
// Actual password length is shorter than string length (31 characters).
#define CAN_PARAM_PASSWORD  138 // 0x8a

// Hardware (FD-capable USB hardware): arbitrary user string, secured for
// OEM versions (string255)
// Write only possible for non-OEM version.
// Actual data length is shorter than string length (119 characters + \0).
#define CAN_PARAM_USERDATA_SECURE  139 // 0x8b

// Hardware (FD-capable USB hardware): arbitrary user string (string255)
// Actual data length is shorter than string length (119 characters + \0).
#define CAN_PARAM_USERDATA  140 // 0x8c

// Hardware: hardware ID for the device as used in Windows (string255)
// String exactly as used in .INF file, in registry, and in device manager.
#define CAN_PARAM_HARDWARE_ID  141 // 0x8d

// Client: process ID of client == calling process of CAN_RegisterClient() (uint32)
#define CAN_PARAM_PROCESS_ID  142 // 0x8e

// Driver: upper limit of hardware handle range (1..MAX_HCANHW) (uint32)
#define CAN_PARAM_MAX_HCANHW  143 // 0x8f

// Driver: upper limit of net handle range (1..MAX_HCANNET) (uint32)
#define CAN_PARAM_MAX_HCANNET  144 // 0x90

// Driver: upper limit of client handle range (1..MAX_HCANCLIENT) (uint32)
#define CAN_PARAM_MAX_HCANCLIENT  145 // 0x91

// Driver: hardware handle enumeration method (uint32)
// 0 = Legacy hardware enumeration: 16..1, 17..MAX_HCANHW (default)
// 1 = Linear hardware enumeration: 1..MAX_HCANHW
#define CAN_PARAM_HW_ENUMERATION_LINEAR  146 // 0x92

// Pause between two transmit messages in microseconds (uint32)
// Client: current pause value for client
// Hardware: supported value range (read-only)
#define CAN_PARAM_XMT_INTERFRAME_DURATION  147 // 0x93

// Hardware handle of a net, as set in net registration. Valid even if associated hardware is
// not plugged in. See also CAN_PARAM_NETHW_ACTIVE (uint32)
#define CAN_PARAM_NETHW_PRESET  148 // 0x94

// API: Debug Log level (uint32)
// 0 = disabled
// 1 = calls only (w/o GetSystemTime)
// 2 = calls (w/o GetSystemTime), parameters, entry/exit, return values
// 3 = calls (including GetSystemTime), parameters, entry/exit, return values
#define CAN_PARAM_DEBUGLOG_LEVEL  149 // 0x95

// API: Path of Debug Log file, without file name (string255, UTF-8)
// Empty string switches off Log file generation, only OutputDebugString output
#define CAN_PARAM_DEBUGLOG_PATH  150 // 0x96

// Hardware: List of protocols supported with current settings (uint32)
// Bitwise OR of CAN_CONST_PROTOCOL_* constants
#define CAN_PARAM_PROTOCOLS  154 // 0x9a


// Parameters that set/return structured data

// Driver/API: PCAN driver or API DLL version (can_param_version_t)
#define CAN_PARAM_VERSION  200 // 0xc8

// Hardware: version of device firmware (can_param_version_t)
#define CAN_PARAM_FIRMWARE_VERSION  201 // 0xc9

// Hardware: version of device bootloader firmware (can_param_version_t)
#define CAN_PARAM_BOOTLOADER_VERSION  202 // 0xca

// Hardware: version of CPLD firmware (can_param_version_t)
#define CAN_PARAM_CPLD_VERSION  203 // 0xcb

// Hardware: revision level of hardware device (can_param_version_t)
#define CAN_PARAM_HARDWARE_VERSION  204 // 0xcc

// Hardware/net: CAN non-FD bit rates in terms of CiA specifications (can_param_bitrate_nom_t)
#define CAN_PARAM_BITRATE_NOM  205 // 0xcd

// Hardware/net: CAN FD bit rates in terms of CiA specifications (can_param_bitrate_fd_t)
#define CAN_PARAM_BITRATE_FD  206 // 0xce 

// Net/client: Acceptance Filter, works on message ID, with binary
// code and mask patterns (can_param_acceptance_filter_t)
#define CAN_PARAM_ACCEPTANCE_FILTER  207 // 0xcf

// Net/client: statistic for received and transmitted messages (can_param_traffic_t)
#define CAN_PARAM_TRAFFIC  208 // 0xd0

// Driver: version and copyright information of a device driver (string255)
#define CAN_PARAM_VERSIONINFO  209 // 0xd1

// Hardware (FD-capable USB/PCI hardware): IRQ delay and timeout for buffered designs
// (IRQ throttle) (can_param_irq_timing_t)
// For good throughput, set count_limit and time_limit high:
// -> buffers are better filled, less IRQs occur and system load is lower.
// For good response timing, set count_limit and time_limit low:
// -> more IRQs occur, delay shrinks, buffers get less filled and system load is higher.
#define CAN_PARAM_IRQ_TIMING  211 // 0xd3

// Hardware (PCAN-Chip USB): Operation mode of 32 digital I/O pins (can_param_io_t)
// Bit mask with pin configuration: 0 = input, 1 = output
#define CAN_PARAM_IO_DIGITAL_CONFIG  213 // 0xd5

// Hardware (PCAN-Chip USB): Value assigned to 32 digital I/O pins (can_param_io_t)
// Bit mask with input/output level: 0 = low level voltage, 1 = high level
#define CAN_PARAM_IO_DIGITAL_VALUE  214 // 0xd6

// Hardware (PCAN-Chip USB): Sets multiple digital I/O pins to high level (can_param_io_t)
// Bit mask: 0 = no effect on pin, 1 = set to high level (write-only)
#define CAN_PARAM_IO_DIGITAL_SET  215 // 0xd7

// Hardware (PCAN-Chip USB): Clears multiple digital I/O pins to low level (can_param_io_t)
// Bit mask: 0 = no effect on pin, 1 = set to low level (write-only)
#define CAN_PARAM_IO_DIGITAL_CLEAR  216 // 0xd8

// Hardware (PCAN-Chip USB): Value of a single analog input pin (read-only) (can_param_io_t)
#define CAN_PARAM_IO_ANALOG_VALUE  217 // 0xd9


// Other constants

#define MAX_HCANHW      64      // only hardware 1 .. MAX_HCANHW permitted
#define MAX_HCANNET     64      // only nets 1 .. MAX_HCANNET permitted
#define MIN_HCANNET_FD  33      // CAN FD nets permitted from handle 33 .. MAX_HCANNET
#define MAX_HCANCLIENT  64      // only clients 1 .. MAX_HCANCLIENT permitted

#define CAN_PARAM_OBJCLASS_DRIVER     0x1    // = 1. field objclass: driver parameter
#define CAN_PARAM_OBJCLASS_HARDWARE   0x2    // = 2. field objclass: hardware parameter
#define CAN_PARAM_OBJCLASS_NET        0x3    // = 3. field objclass: net parameter
#define CAN_PARAM_OBJCLASS_CLIENT     0x4    // = 4. field objclass: client parameter
#define CAN_PARAM_OBJCLASS_API        0x5    // = 5. field objclass: API parameter

#define CAN_PARAM_MAX_HARDWARENAMELEN 33     // length of a hardware name: 32 characters + terminator
#define CAN_PARAM_MAX_NETNAMELEN      21     // length of a net name: 20 characters + terminator
#define CAN_PARAM_MAX_CLIENTNAMELEN   21     // length of a client name: 20 characters + terminator
#define CAN_PARAM_CONST_MAX_STRINGLEN 256    // size of info fields: 255 bytes + terminator

#define CAN_CONST_CAN_DATA_COUNT      8      // number of data bytes in a CAN message frame
#define CAN_CONST_CANFD_DATA_COUNT    64     // deprecated, use CAN_CONST_FD_DATA_COUNT
#define CAN_CONST_FD_DATA_COUNT       64     // number of data bytes in a CAN FD message frame
#define CAN_CONST_MAX_INFOLEN         256    // length of an info string: 255 characters + terminator

// Protocol types for CAN_PARAM_PROTOCOLS
#define CAN_CONST_PROTOCOL_CAN20A     0x1    // = 1. std CAN 11-bit ID
#define CAN_CONST_PROTOCOL_CAN20B     0x2    // = 2. std CAN 29-bit ID
#define CAN_CONST_PROTOCOL_CAN20AB    (CAN_CONST_PROTOCOL_CAN20A | CAN_CONST_PROTOCOL_CAN20B) 
                                             // = 3. std CAN 11- & 29-bit ID (mask for CAN 2.0 capabilities)
#define CAN_CONST_PROTOCOL_FD_BOSCH   0x4    // = 4. CAN FD, first Bosch definition
#define CAN_CONST_PROTOCOL_FD_ISO     0x8    // = 8. CAN FD, later ISO definition
#define CAN_CONST_PROTOCOL_FD_ANY     (CAN_CONST_PROTOCOL_FD_BOSCH | CAN_CONST_PROTOCOL_FD_ISO)
                                             // = 12. CAN FD, either ISO or Bosch (mask for CAN FD capabilites)

// Record types

#define CAN_INVALID_RECORDTYPE                  0x0     // Guaranteed not to be a valid record type
#define CAN_RECORDTYPE_basedata                 0x1000  // Abstract base class.
                                                        // Identical header for all data records ("msg..." and "event_..."),
                                                        // so all records can be cast to this
#define CAN_RECORDTYPE_basemsg                  0x1001  // Abstract base class.
                                                        // Identical header for records "msg", "msg_rtr", "msg_fd"
                                                        // so all message records can be cast to this
#define CAN_RECORDTYPE_msg                      0x1002  // CAN message 11/29-bit ID; non-FD, 8 data bytes
#define CAN_RECORDTYPE_msg_rtr                  0x1003  // CAN RTR message 11/29-bit ID
#define CAN_RECORDTYPE_msg_fd                   0x1004  // Represents a CAN bus message with FDF bit set.
                                                        // ID = 11/29-bit ID; max. 64 data bytes
                                                        // identical with standard CAN message up to the ID field
#define CAN_RECORDTYPE_hwstatus                 0x1101  // Message related errors (as OVERRUN, BUSWARNING, BUSPASSIVE, BUSOFF).
                                                        // Also used as error feedback for CAN_Write()
#define CAN_RECORDTYPE_errorframe               0x1102  // Error frame from CAN core, mapped to SJA1000 ECC error code capture
#define CAN_RECORDTYPE_errorcounter_decrement   0x1103  // Some error counter decremented
#define CAN_RECORDTYPE_busload                  0x1104  // CAN bus load in percent
#define CAN_RECORDTYPE_event_pnp                0x1301  // Plug&Play event from driver (hardware connect/disconnect detected)
#define CAN_RECORDTYPE_event_fd_error           0x1302  // Incompatibility between FD net and non-FD hardware
#define CAN_RECORDTYPE_event_param              0x1303  // A hardware or net parameter was changed, all clients get notified

// Types of parameter records

#define CAN_RECORDTYPE_param_base               0xfff   // Common to all parameter set/get records
#define CAN_RECORDTYPE_param_uint32             0xffd   // 32-bit unsigned integer
#define CAN_RECORDTYPE_param_uint64             0xffb   // 64-bit unsigned integer
#define CAN_RECORDTYPE_param_string255          0xff9   // string with 255 byte characters + terminating null character
// Record types for parameters that set/return structured data
#define CAN_RECORDTYPE_param_version            0xff8   // Generic record to hold all version informations
#define CAN_RECORDTYPE_param_bitrate_btr0btr1   0x7     // See also CAN_PARAM_BITRATE
#define CAN_RECORDTYPE_param_bitrate_nom        0xcd    // See also CAN_PARAM_BITRATE_NOM
#define CAN_RECORDTYPE_param_bitrate_fd         0xce    // See also CAN_PARAM_BITRATE_FD
#define CAN_RECORDTYPE_param_acceptance_filter  0xcf    // See also CAN_PARAM_ACCEPTANCE_FILTER
#define CAN_RECORDTYPE_param_buserrorgeneration 0x6e    // See also CAN_PARAM_BUSERRORGENERATION
#define CAN_RECORDTYPE_param_traffic            0xd0    // See also CAN_PARAM_TRAFFIC
#define CAN_RECORDTYPE_param_irq_timing         0xd3    // See also CAN_PARAM_IRQ_TIMING
#define CAN_RECORDTYPE_param_io                 0xff7   // Generic access to digital and analog I/O pins
#define CAN_RECORDTYPE_param_io_digital_config  0xd5    // See also CAN_PARAM_IO_DIGITAL_CONFIG
#define CAN_RECORDTYPE_param_io_digital_value   0xd6    // See also CAN_PARAM_IO_DIGITAL_VALUE
#define CAN_RECORDTYPE_param_io_digital_set     0xd7    // See also CAN_PARAM_IO_DIGITAL_SET
#define CAN_RECORDTYPE_param_io_digital_clear   0xd8    // See also CAN_PARAM_IO_DIGITAL_CLEAR
#define CAN_RECORDTYPE_param_io_analog_value    0xd9    // See also CAN_PARAM_IO_ANALOG_VALUE


// Message types

#define CAN_MSGTYPE_STANDARD       0x0000    // standard data frame (11-bit ID)
#define CAN_MSGTYPE_EXTENDED       0x0001    // 1, if extended data frame (CAN 2.0B, 29-bit ID)
#define CAN_MSGTYPE_SELFRECEIVE    0x0002    // 1, if message shall be/has been self-received by the client
#define CAN_MSGTYPE_HW_SELFRECEIVE 0x0004    // 1, if self-receive was performed by hardware. 0, if hardware is incapable of self-receive
#define CAN_MSGTYPE_SINGLESHOT     0x0008    // 1, if no re-transmission shall be performed for the message (self ACK)
#define CAN_MSGTYPE_BRS            0x0010    // Bit Rate Switch: 1, if CAN FD frame data was sent with higher bit rate
#define CAN_MSGTYPE_ESI            0x0020    // Error State Indicator: 1, if CAN FD transmitter was error active

// Type definitions

typedef UINT8  HCANHW;        // type 'hardware handle'
typedef UINT8  HCANNET;       // type 'net handle'
typedef UINT8  HCANCLIENT;    // type 'client handle'
typedef UINT8  HCANOBJECT;    // any handle type
typedef UINT32 can_status_t;  // status value/return code

// PCAN device types
typedef enum can_device
{
    pcan_unknown,
    pcan_isa,
    pcan_pci,
    pcan_usb,
    pcan_pccard,
    pcan_virtual,
    pcan_lan,
    pcan_dng,
    dci_can
} can_device_t;


#pragma pack(push, 1)         // the following records are byte-aligned

// common record header
typedef struct can_recordheader_struct
{
    UINT16  size;  // #0 +0x00  absolute length of record in bytes
    UINT16  type;  // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
} can_recordheader_t;

// Record "basedata"
//      type = CAN_RECORDTYPE_basedata = 0x1000
// hierarchy = Is a base record for these other records:
//                 "basemsg", "hwstatus", "errorframe", "errorcounter_decrement", "busload",
//                 "event_pnp", "event_fd_error", "event_param".
//      size = 22 = 0x16 bytes, aligned to 1 bytes
//      info = Abstract base class
//             Identical header for all data records ("msg..." and "event_..."),
//             so all records can be cast to this
//
typedef struct can_basedata_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
} can_basedata_t;

// Record "basemsg"
//      type = CAN_RECORDTYPE_basemsg = 0x1001
// hierarchy = Inherits fields from base record "basedata".
//             Is a base record for these other records: 
//                 "msg", "msg_rtr", "msg_fd".
//      size = 29 = 0x1d bytes, aligned to 1 bytes
//      info = Abstract base class.
//             Identical header for records "msg", "msg_rtr", "msg_fd"
//             so all message records can be cast to this
//
typedef struct can_basemsg_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    UINT16       msgtype;     // #6 +0x16  message flags. See CAN_MSGTYPE_... constants
    UINT32       id;          // #7 +0x18  11/29-bit CAN identifier
    UINT8        dlc;         // #8 +0x1C  physical Data Length Code, encodes count of bytes
} can_basemsg_t;

// Record "msg"
//      type = CAN_RECORDTYPE_msg = 0x1002
// hierarchy = Inherits fields from base record "basemsg".
//      size = 37 = 0x25 bytes, aligned to 1 bytes
//      info = CAN message 11/29-bit ID; non-FD, 8 data bytes
//
typedef struct can_msg_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    UINT16       msgtype;     // #6 +0x16  message flags. See CAN_MSGTYPE_... constants
    UINT32       id;          // #7 +0x18  11/29-bit CAN identifier
    UINT8        dlc;         // #8 +0x1C  physical Data Length Code, encodes count of bytes
    union {                   // #9 +0x1D  data bytes (8)
        UINT8   data[CAN_CONST_CAN_DATA_COUNT];
        UINT16  data_x2[4];
        UINT32  data_x4[2];
        UINT64  data_x8[1];
    };
} can_msg_t;

// Record "msg_rtr"
//      type = CAN_RECORDTYPE_msg_rtr = 0x1003
// hierarchy = Inherits fields from base record "basemsg".
//      size = 29 = 0x1d bytes, aligned to 1 bytes
//      info = CAN RTR message 11/29-bit ID
//
typedef struct can_msg_rtr_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    UINT16       msgtype;     // #6 +0x16  message flags. See CAN_MSGTYPE_... constants
    UINT32       id;          // #7 +0x18  11/29-bit CAN identifier
    UINT8        dlc;         // #8 +0x1C  physical Data Length Code, encodes count of bytes
} can_msg_rtr_t;

// Record "msg_fd"
//      type = CAN_RECORDTYPE_msg_fd = 0x1004
// hierarchy = Inherits fields from base record "basemsg".
//      size = 93 = 0x5d bytes, aligned to 1 bytes
//      info = Represents a CAN bus message with FDF bit set.
//             ID = 11/29-bit; max. 64 data bytes
//
typedef struct can_msg_fd_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    UINT16       msgtype;     // #6 +0x16  message flags. See CAN_MSGTYPE_... constants
    UINT32       id;          // #7 +0x18  11/29-bit CAN identifier
    UINT8        dlc;         // #8 +0x1C  physical Data Length Code, encodes count of bytes
    union {                   // #9 +0x1D  data bytes (64)
        UINT8   data[CAN_CONST_FD_DATA_COUNT];
        UINT16  data_x2[32];
        UINT32  data_x4[16];
        UINT64  data_x8[8];
    };
} can_msg_fd_t;

// Record "hwstatus"
//      type = CAN_RECORDTYPE_hwstatus = 0x1101
// hierarchy = Inherits fields from base record "basedata".
//      size = 26 = 0x1a bytes, aligned to 1 bytes
//      info = Message related errors (as OVERRUN, BUSWARNING, BUSPASSIVE, BUSOFF)
//             Also used as error feedback for CAN_Write()
//
typedef struct can_hwstatus_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    can_status_t status;      // #6 +0x16  CAN status code
} can_hwstatus_t;

// Record "errorframe"
//      type = CAN_RECORDTYPE_errorframe = 0x1102
// hierarchy = Inherits fields from base record "basedata".
//      size = 28 = 0x1c bytes, aligned to 1 bytes
//      info = Error frame from the CAN core, mapped to SJA1000 ECC error code capture 
//
typedef struct can_errorframe_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    UINT8        errortype;   // #6 +0x16  0x01 = bit error, 0x02 = form error, 0x04 = stuff error, 0x08 = other
    UINT8        direction;   // #7 +0x17  1, if error occurred on RCV, 0 for XMT
    UINT8        ecc;         // #8 +0x18  Error Code Capture, error position in bit stream. Defined as 'SAJ1000.ECC & 0x1f'
    UINT8        rxErrCount;  // #9 +0x19  Receive error counter value, caused by standard messages
    UINT8        txErrCount;  // #10 +0x1A  Transmit error counter value, caused by standard messages
    UINT8        errorsource; // #11 +0x1B  0 = CAN bus, 1 = error generator
} can_errorframe_t;

// Record "errorcounter_decrement"
//      type = CAN_RECORDTYPE_errorcounter_decrement = 0x1103
// hierarchy = Inherits fields from base record "basedata".
//      size = 24 = 0x18 bytes, aligned to 1 bytes
//      info = Some error counter was decremented
//
typedef struct can_errorcounter_decrement_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    UINT8        rxErrCount;  // #6 +0x16  Receive error counter value, caused by standard messages
    UINT8        txErrCount;  // #7 +0x17  Transmit error counter value, caused by standard messages
} can_errorcounter_decrement_t;

// Record "busload"
//      type = CAN_RECORDTYPE_busload = 0x1104
// hierarchy = Inherits fields from base record "basedata".
//      size = 24 = 0x18 bytes, aligned to 1 bytes
//      info = CAN bus load in percent
//
typedef struct can_busload_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message/event is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    UINT16       busload;     // #6 +0x16  bus load in percent
} can_busload_t;

// Record "event_pnp"
//      type = CAN_RECORDTYPE_event_pnp = 0x1301
// hierarchy = Inherits fields from base record "basedata".
//      size = 285 = 0x11d bytes, aligned to 1 bytes
//      info = Plug&Play event from driver
//             Hardware connect/disconnect detected
//
typedef struct can_event_pnp_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message/event is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    HCANHW       hw;          // #6 +0x16  handle of hardware
    UINT32       plug_type;   // #7 +0x17  0 = plug-out, 1 = plug-in
    HCANNET      hw_net;      // #8 +0x1b  hardware is connected to this net
    char         info[CAN_CONST_MAX_INFOLEN+1];
                              // #9 +0x1c  additional info text
} can_event_pnp_t;

// Record "event_fd_error" ***
//      type = CAN_RECORDTYPE_event_fd_error = 0x1302
// hierarchy = Inherits fields from base record "basedata".
//      size = 281 = 0x119 bytes, aligned to 1 bytes
//      info = Incompatibility between FD net and non-FD hardware
//
typedef struct can_event_fd_error_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message/event is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    HCANHW       hw;          // #6 +0x16  handle of hardware
    HCANNET      hw_net;      // #7 +0x17  hardware was supposed to connect to this net
    char  info[CAN_CONST_MAX_INFOLEN+1];
                              // #8 +0x18  additional info text
} can_event_fd_error_t;

// Record "event_param" ***
//      type = CAN_RECORDTYPE_event_param = 0x1303
// hierarchy = Inherits fields from base record "basedata".
//      size = 291 = 0x123 bytes, aligned to 1 bytes
//      info = A hardware or net parameter was changed, all clients get notified.
//             (field 'net' not used)
//
typedef struct can_event_param_struct
{
    UINT16       size;        // #0 +0x00  absolute length of record in bytes
    UINT16       type;        // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT64       timestamp;   // #2 +0x04  record timestamp in microseconds system time, 64-bit
    UINT64       tag;         // #3 +0x0C  space for application-specific data (object pointer)
    HCANCLIENT   client;      // #4 +0x14  PCAN client handle (sender, if message/event is received)
    HCANNET      net;         // #5 +0x15  PCAN net handle
    UINT32       objclass;    // #6 +0x16  class of the object that changed a parameter (see CAN_PARAM_OBJCLASS_... constants)
    UINT32       objhandle;   // #7 +0x1A  handle of the object that changed a parameter (0 for driver)
    UINT32       parameter;   // #8 +0x1E  code/recordtype of changed parameter
                              //           parameter belongs to class <objclass> and handle <objhandle>
    char  info[CAN_CONST_MAX_INFOLEN+1];
                              // #9 +0x22  additional info text
} can_event_param_t;


// can_any_record_t: Union for all types of records
typedef union
{
    can_recordheader_t   header;  // identical for all records
    can_basedata_t       basedata;
    can_basemsg_t        basemsg;
    can_msg_t            msg;
    can_msg_rtr_t        msg_rtr;
    can_msg_fd_t         msg_fd;
    can_hwstatus_t       hwstatus;
    can_errorframe_t     errorframe;
    can_errorcounter_decrement_t  errorcounter_decrement;
    can_busload_t        busload;
    can_event_pnp_t      event_pnp;
    can_event_fd_error_t event_fd_error;
    can_event_param_t    event_param;
} can_any_record_t;


// Other records

// Record "available_hardware"
// hierarchy = Not inherited from any other record.
//      size = 50 = 0x32 bytes, aligned to 1 bytes
//      info = Data of available hardware. Returned from CAN_GetAvailableHardware.
//
typedef struct can_available_hardware
{
    can_device_t device;      // #0 +0x00  device
    HCANHW       hw;          // #1 +0x04  hardware handle
    char         name[CAN_PARAM_MAX_HARDWARENAMELEN];
                              // #2 +0x05  hardware name
    UINT32       type;        // #3 +0x26  hardware type, see also CAN_PARAM_HWDRIVERNR
    UINT32       channel;     // #4 +0x2A  channel number (1-based index)
    UINT32       reserved;    // #5 +0x2E  reserved for future use
} can_available_hardware_t;


// Parameters

// Record "param_base"
//      type = CAN_RECORDTYPE_param_base = 0xfff
// hierarchy = Is the base record for all other can_param_... records
//      size = 12 = 0xc bytes, aligned to 1 bytes
//      info = Common to all parameter set/get records
//
typedef struct can_param_base_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
} can_param_base_t;

// Record "param_uint32"
//      type = CAN_RECORDTYPE_param_uint32 = 0xffd
// hierarchy = Inherits fields from base record "param_base".
//      size = 16 = 0x10 bytes, aligned to 1 bytes
//
typedef struct can_param_uint32_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT32  value;            // #4 +0x0C  an unsigned 32-bit numeric value
} can_param_uint32_t;

// Record "param_uint64"
//      type = CAN_RECORDTYPE_param_uint64 = 0xffb
// hierarchy = Inherits fields from base record "param_base".
//      size = 20 = 0x14 bytes, aligned to 1 bytes
//
typedef struct can_param_uint64_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT64  value;            // #4 +0x0C  an unsigned 64-bit numeric value
} can_param_uint64_t;

// Record "param_string255"
//      type = CAN_RECORDTYPE_param_string255 = 0xff9
// hierarchy = Inherits fields from base record "param_base".
//      size = 268 = 0x10c bytes, aligned to 1 bytes
//
typedef struct can_param_string255_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    char    value[CAN_PARAM_CONST_MAX_STRINGLEN];
                              // #4 +0x0C  (string data type logic) a 255 char string, used for info fields
} can_param_string255_t;

// Record "param_version"
//      type = CAN_RECORDTYPE_param_version = 0xff8
// hierarchy = Inherits fields from base record "param_base".
//      size = 32 = 0x20 bytes, aligned to 1 bytes
//      info = Generic record to hold all version informations.
//             Version info is held as <major>.<minor>.<revision>-<build> <debug/release>
//             Used with parameters CAN_PARAM_VERSION, CAN_PARAM_HARDWARE_VERSION, CAN_PARAM_CPLD_VERSION,
//             CAN_PARAM_FIRMWARE_VERSION, CAN_PARAM_BOOTLOADER_VERSION
//
typedef struct can_param_version_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT32  major;            // #4 +0x0C  significant changes, new product; change means compatibility issue
    UINT32  minor;            // #5 +0x10  functional changes; change means downward compatibility
    UINT32  revision;         // #6 +0x14  patch level, bug fixes; change should not raise a compatibility issue
    UINT32  build;            // #7 +0x18  sequential compile count, unique ID for each binary
    UINT32  debug;            // #8 +0x1C  0=release, 1=debug build
} can_param_version_t;

// Record "param_bitrate_btr0btr1"
//      type = CAN_RECORDTYPE_param_bitrate_btr0btr1 = 0x7
// hierarchy = Inherits fields from base record "param_uint32".
//      size = 16 = 0x10 bytes, aligned to 1 bytes
//      info = CAN_PARAM_BITRATE
//             Defines bit rates in terms SJA1000 timing register BTR0BTR1 values
//
typedef struct can_param_bitrate_btr0btr1_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT32  value;            // #4 +0x0C  an unsigned numeric value
} can_param_bitrate_btr0btr1_t ;

// Record "param_bitrate_nom"
//      type = CAN_RECORDTYPE_param_bitrate_nom = 0xcd
// hierarchy = Inherits fields from base record "param_base".
//      size = 33 = 0x21 bytes, aligned to 1 bytes
//      info = Defines CAN non-FD bit rates in terms of CiA specifications
//             Same as driver-internal representation
//
typedef struct can_param_bitrate_nom_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT32  f_core;           // #4 +0x0C  clock of CAN state machine. time_quantum = brp / f_core
    UINT32  brp;              // #5 +0x10  clock prescaler for nominal time quantum
    UINT32  tseg1;            // #6 +0x14  tseg1 segment for nominal bit rate in time quanta. tseg1 = prop_seg + phase1_seg
    UINT32  tseg2;            // #7 +0x18  tseg2 segment for nominal bit rate in time quanta
    UINT32  sjw;              // #8 +0x1C  Synchronization Jump Width for nominal bit rate in time quanta
    UINT8   sam;              // #9 +0x20  1 = SJA1000 could set 3x sampling, 0 = 1x sampling. Not used in FPGA-based hardware
} can_param_bitrate_nom_t;

// Record "param_bitrate_fd"
//      type = CAN_RECORDTYPE_param_bitrate_fd = 0xce
// hierarchy = Inherits fields from base record "param_base".
//      size = 53 = 0x35 bytes, aligned to 1 bytes
//      info = Defines CAN FD bit rates in terms of CiA specifications
//             Same as driver-internal representation
//
typedef struct can_param_bitrate_fd_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT32  f_core;           // #4 +0x0C  clock of CAN state machine. time_quantum = brp / f_core
    UINT32  nom_brp;          // #5 +0x10  clock prescaler for nominal time quantum. time_quantum = brp / f_core
    UINT32  nom_tseg1;        // #6 +0x14  tseg1 segment for nominal bit rate in time quanta. tseg1 = prop_seg + phase1_seg
    UINT32  nom_tseg2;        // #7 +0x18  tseg2 segment for nominal bit rate in time quanta
    UINT32  nom_sjw;          // #8 +0x1C  Synchronization Jump Width for nominal bit rate in time quanta
    UINT8   nom_sam;          // #9 +0x20  1 = SJA1000 could set 3x sampling, 0 = 1x sampling. Not used in FPGA-based hardware
    UINT32  data_brp;         // #10 +0x21  clock prescaler for high-speed data time quantum. time_quantum = brp / f_core
    UINT32  data_tseg1;       // #11 +0x25  tseg1 segment for fast data bit rate in time quanta. tseg1 = prop_seg + phase1_seg
    UINT32  data_tseg2;       // #12 +0x29  tseg2 segment for fast data bit rate in time quanta
    UINT32  data_sjw;         // #13 +0x2D  Synchronization Jump Width for high-speed data bit rate in time quanta
    UINT32  data_ssp_offset;  // #14 +0x31  secondary sample point delay for high-speed data bit rate in f_cancore cycles. Not used in FPGA-based hardware
} can_param_bitrate_fd_t;

// Record "param_acceptance_filter"
//      type = CAN_RECORDTYPE_param_acceptance_filter = 0xcf
// hierarchy = Inherits fields from base record "param_base".
//      size = 22 = 0x16 bytes, aligned to 1 bytes
//      info = Defines a single acceptance filter.
//             Filter works on message ID, with binary code and mask patterns.
//
typedef struct can_param_acceptance_filter_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT8   index;            // #4 +0x0C  number of filter, 0 .. CAN_PARAM_ACCFILTER_COUNT-1
    UINT8   extended;         // #5 +0x0D  0 = 11-bit filter, 1 = 29-bit
    UINT32  code;             // #6 +0x0E  message ID must match 'code' bit positions where 'mask' = 1
    UINT32  mask;             // #7 +0x12  message ID must match 'code' bit positions where 'mask' = 1
} can_param_acceptance_filter_t;

// Record "param_buserrorgeneration"
//      type = CAN_RECORDTYPE_param_buserrorgeneration = 0x6e
// hierarchy = Inherits fields from base record "param_base".
//      size = 24 = 0x18 bytes, aligned to 1 bytes
//      info = CAN_PARAM_BUSERRORGENERATION
//             Create errors on bus (PCAN-USB Pro and FD-capable PCAN hardware)
//
typedef struct can_param_buserrorgeneration_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT16  mode;             // #4 +0x0C  0 = off, 1 = repeated, 2 = single
    UINT16  bit_pos;          // #5 +0x0E  bit position
    UINT32  id;               // #6 +0x10  in repeat mode: CAN ID (11-bit and 29-bit)
    UINT16  ok_counter;       // #7 +0x14  in repeat mode: number of successive CAN messages to leave untouched
    UINT16  error_counter;    // #8 +0x16  in repeat mode: number of successive CAN messages to disrupt
} can_param_buserrorgeneration_t;

// Record "param_traffic"
//      type = CAN_RECORDTYPE_param_traffic = 0xd0
// hierarchy = Inherits fields from base record "param_base".
//      size = 36 = 0x24 bytes, aligned to 1 bytes
//      info = Statistics for received and transmitted messages
//
typedef struct can_param_traffic_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT32  rcvmsgcnt;        // #4 +0x0C  total number of received messages
    UINT32  rcvbitcnt;        // #5 +0x10  total number of received bits
    UINT32  xmtmsgcnt;        // #6 +0x14  total number of transmitted messages
    UINT32  xmtbitcnt;        // #7 +0x18  total number of transmitted bits
    UINT32  msgcnt;           // #8 +0x1C  total number of received and transmitted messages
    UINT32  bitcnt;           // #9 +0x20  total number of received and transmitted bits
} can_param_traffic_t;

// Record "param_irq_timing"
//      type = CAN_RECORDTYPE_param_irq_timing = 0xd3
// hierarchy = Inherits fields from base record "param_base".
//      size = 20 = 0x14 bytes, aligned to 1 bytes
//      info = IRQ delay and timeout for buffered designs (IRQ throttle).
//             For good throughput, set count_limit and time_limit high:
//             -> buffers are better filled, less IRQs occur and system load is lower.
//             For good response timing, set count_limit and time_limit low:
//             -> more IRQs occur, delay shrinks, buffers get less filled and system load is higher.
//
typedef struct can_param_irq_timing_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT32  count_limit;      // #4 +0x0C  trigger IRQ at least every <count_limit> records
    UINT32  time_limit;       // #5 +0x10  trigger IRQ at least every <time_limit> microseconds
} can_param_irq_timing_t;

// Record "param_io"
//      type = CAN_RECORDTYPE_param_io = 0xff7
// hierarchy = Inherits fields from base record "param_base".
//      size = 20 = 0x14 bytes, aligned to 1 bytes
//      info = Generic access to digital and analog I/O pins.
//             Used for configuration, set, and get, with parameters 
//             CAN_PARAM_IO_DIGITAL_CONFIG, CAN_PARAM_IO_DIGITAL_VALUE, CAN_PARAM_IO_DIGITAL_SET,
//             CAN_PARAM_IO_DIGITAL_CLEAR, CAN_PARAM_IO_ANALOG_VALUE
//
typedef struct can_param_io_struct
{
    UINT16  size;             // #0 +0x00  absolute length of record in bytes
    UINT16  type;             // #1 +0x02  type code of the record (only LSBs with mask 0x3fff)
    UINT32  objclass;         // #2 +0x04  class of the object this parameter belongs to (see CAN_PARAM_OBJCLASS_... constants)
    UINT32  objhandle;        // #3 +0x08  handle of the object this parameter belongs to (0 for driver)
    UINT32  index;            // #4 +0x0C  index of 32-bit bank to modify; must be 0, reserved for future use
    UINT32  value;            // #5 +0x10  generic field: bit mask for 32 digital pins, or single analog value
} can_param_io_t;


// can_any_param_t: Union for all types of parameter records
typedef union
{
    can_recordheader_t           header; // identical for all records
    can_param_base_t             base;
    can_param_uint32_t           uint32;
    can_param_uint64_t           uint64;
    can_param_string255_t        string255;
    can_param_version_t          version;
    can_param_bitrate_btr0btr1_t bitrate_btr0btr1;
    can_param_bitrate_nom_t      bitrate_nom;
    can_param_bitrate_fd_t       bitrate_fd;
    can_param_acceptance_filter_t  acceptance_filter;
    can_param_buserrorgeneration_t buserrorgeneration;
    can_param_traffic_t          traffic;
    can_param_irq_timing_t       irq_timing;
    can_param_io_t               io;
} can_any_param_t;


// can_any_bitrate_t: Union for all types of bit rate records
typedef union
{
    can_recordheader_t           header; // identical for all records
    can_param_base_t             base;
    can_param_bitrate_btr0btr1_t bitrate_btr0btr1;
    can_param_bitrate_nom_t      bitrate_nom;
    can_param_bitrate_fd_t       bitrate_fd;
} can_any_bitrate_t;


#pragma pack(pop)    // default alignment again


// Function prototypes


//-----------------------------------------------------------------------------
// CAN_RegisterNet()
//   Adds a net to the driver's net list.
// Parameters:
//   device:   PCAN device to be used.
//   net:      Requested handle of the PCAN net. If 0, the net handle is
//             determined automatically.
//   name:     Name of the PCAN net to be registered.
//   hw:       Related CAN hardware handle. If an internal net shall be defined
//             and no hardware shall be connected, this value must be 0.
//   bitrate:  Pointer to bit rate record.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLNET, ILLHW, ILLPARAMVAL.

can_status_t __stdcall CAN_RegisterNet(
        can_device_t       device,
        HCANNET            net,
        LPCSTR             name,
        HCANHW             hw,
        can_any_bitrate_t  *bitrate);


//-----------------------------------------------------------------------------
// CAN_RemoveNet()
//   Deletes a net definition from the driver's net list.
// Parameters:
//   device: PCAN device to be used.
//   net:    Handle of the net to be removed.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLNET, NETINUSE.

can_status_t __stdcall CAN_RemoveNet(
        can_device_t device,
        HCANNET      net);


//-----------------------------------------------------------------------------
// CAN_RegisterClient()
//   Registers a client at the device driver. Creates a client handle and
//   allocates the receive queue (only one per client). The wnd parameter
//   can be 0 for Console Applications. The client does not receive any
//   messages until either RegisterMessages() or SetClientFilter() is called.
// Parameters:
//   device: PCAN device to be used.
//   name:   Name of the client.
//   wnd:    Window handle of the client (only for information purposes).
//   client: [out] Returns the handle that was assigned to the client.
// Returns:
//   Error code. Possible errors: NODRIVER, RESOURCE.

can_status_t __stdcall CAN_RegisterClient(
        can_device_t device,
        LPCSTR       name,
        UINT32       wnd,
        HCANCLIENT*  client);


//-----------------------------------------------------------------------------
// CAN_RemoveClient()
//   Removes a client from the client list in the device driver. Free all
//   resources (receive/transmit queues etc.)
//   Each call of this function can change the filter of the connected
//   hardware, so that the CAN controller must be reset.
// Parameters:
//   device: PCAN device to be used.
//   client: Handle of the client.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLCLIENT.

can_status_t __stdcall CAN_RemoveClient(
        can_device_t device,
        HCANCLIENT   client);


//-----------------------------------------------------------------------------
// CAN_ConnectToNet()
//   Connects a client to a net.
//   The net is assigned by its name. The hardware is initialized with the
//   Bit rate if it is the first client which connects to the net.
//   If the hardware is already in use by another net, the connection fails and
//   the error ERR_HWINUSE will be returned.
// Parameters:
//   device:  PCAN device to be used.
//   client:  Handle of the client to be connected to the net.
//   netName: Name of the net with which the client shall be connected.
//   net:     [out] If successful, returns the handle of the net, with which the 
//            client has been connected.
// Returns:
//   Error code.
//   Possible errors: NODRIVER, ILLCLIENT, ILLNET, ILLHW, ILLPARAMVAL, HWINUSE,
//                    REGTEST.

can_status_t __stdcall CAN_ConnectToNet(
        can_device_t device,
        HCANCLIENT   client,
        LPSTR        netName,
        HCANNET      *net);


//-----------------------------------------------------------------------------
// CAN_DisconnectFromNet()
//   Disconnects a client from a net. This means: no more messages will be
//   received by this client. Each call of this function can change the
//   filter of the connected hardware, so that the CAN controller must be
//   reset.
// Parameters:
//   device: PCAN device to be used.
//   client: Handle of the client to be disconnected from a net.
//   net:    Handle of the net, from which the client shall be disconnected.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLCLIENT, ILLNET, REGTEST.

can_status_t __stdcall CAN_DisconnectFromNet(
        can_device_t device,
        HCANCLIENT   client,
        HCANNET      net);


//-----------------------------------------------------------------------------
// CAN_ConnectToHardware()
//   Connects a client to a hardware.
// Parameters:
//   device: PCAN device to be used.
//   client: Handle of the client to be connected to the hardware.
//   params: Parameter string.
//   net:    [out] If successful, returns the handle of the net with which the 
//           client has been connected.
// Returns:
//   Error code.
//   Possible errors: NODRIVER, ILLCLIENT, ILLNET, ILLHW, ILLPARAMVAL, HWINUSE,
//                    REGTEST.

can_status_t __stdcall CAN_ConnectToHardware(
    can_device_t device,
    HCANCLIENT   client,
    LPSTR        params,
    HCANNET      *net);


//-----------------------------------------------------------------------------
// CAN_GetSystemTime()
//   Gets the internal device driver timer value of the Virtual Machine
//   Manager.
// Parameters:
//   device: PCAN device to be used.
//   time:   [out] Returns the timestamp in microseconds.
// Returns:
//   Error code. Possible errors: NODRIVER.

can_status_t __stdcall CAN_GetSystemTime(
        can_device_t device,
        UINT64       *time);


//-----------------------------------------------------------------------------
// CAN_GetHardwareStatus()
//   Gets the current state of a hardware (e.g. ERR_BUSOFF, ERR_OVERRUN, ...)
// Parameters:
//   device: PCAN device to be used.
//   hw:     Handle of the hardware.
// Returns:
//   Error code.
//   Possible errors: NODRIVER, ILLHW, BUSWARNING, BUSPASSIVE, BUSOFF, OVERRUN.

can_status_t __stdcall CAN_GetHardwareStatus(
        can_device_t device,
        HCANHW       hw);


//-----------------------------------------------------------------------------
// CAN_RegisterMessages()
//   Announces that the client wants to receive messages from the net.
//   The acceptance filter of the client will be expanded so that all
//   messages in the specified range will be received.
//   There is only ONE filter for Standard and Extended messages.
//   The Standard messages will be registered as if the ID was built with
//   the bits in positions 28..18.
//   Example: registration of Standard ID 0x400 means that the Extended ID
//   0x10000000 will be also received.
//   If the specified CAN-ID range requires a reconfiguration of the CAN
//   controller, the CAN controller performs a hardware reset cycle.
//   It is not guaranteed that the client only receives the messages with
//   the specified CAN-ID range. The actually received messages depend on
//   the used CAN controller (usually SJA1000).
// Parameters:
//   device:   PCAN device to be used.
//   client:   Handle of the client, for which the reception filter shall be
//             expanded.
//   net:      Handle of the connected PCAN net, for which the reception filter
//             shall be expanded.
//   IDfrom,
//   IDto:     These values specify the message ID range that shall pass the
//             acceptance filter.
//   extended: Specifies whether the parameters IDfrom and IDto contain
//             11-bit IDs (extended = 0) or 29-bit IDs (extended = 1).
// Returns:
//   Error code. Possible errors: NODRIVER, ILLCLIENT, ILLNET, REGTEST.

can_status_t __stdcall CAN_RegisterMessages(
        can_device_t device,
        HCANCLIENT   client,
        HCANNET      net,
        UINT32       IDfrom,
        UINT32       IDto,
        UINT8        extended);


//-----------------------------------------------------------------------------
// CAN_SetClientFilter()
//   Sets the filters of a client, of the connected net, and of the
//   connected hardware.
// Parameters:
//   device:      PCAN device to be used.
//   client:      Handle of the client to be configured.
//   net:         Handle of the net, with which the client is connected.
//   filterIndex: Specifies which filter shall be set. 0 = first filter,
//                1 = second filter (in Dual-Filter-Mode).
//   filterMode:  Not used, must be 0.
//   extended:    Specifies whether the accCode/accMask parameters contain
//                11-bit values (extended = 0), or 29-bit values (extended = 1).
//   accCode,
//   accMask:     The acceptance code and mask are part of the acceptance
//                filter of the SJA1000.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLCLIENT, ILLNET, ILLPARAMVAL.

can_status_t __stdcall CAN_SetClientFilter(
        can_device_t device,
        HCANCLIENT   client,
        HCANNET      net,
        UINT32       filterIndex,
        UINT32       filterMode,
        UINT8        extended,
        UINT32       accCode,
        UINT32       accMask);


//-----------------------------------------------------------------------------
// CAN_ResetClientFilter()
//   Resets the filter of a client.
// Parameters:
//   device: PCAN device to be used.
//   client: Handle of the client, for which the filter shall be reset.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLCLIENT, ILLNET.

can_status_t __stdcall CAN_ResetClientFilter(
        can_device_t device,
        HCANCLIENT   client);


//-----------------------------------------------------------------------------
// CAN_Read()
//   Returns a number of CAN_*-records from the client's receive queue.
//   Records are CAN messages, error events, and other information.
// Parameters:
//   device:      PCAN device to be used.
//   client:      Handle of the client whose receive queue shall be read.
//   buffer:      Pointer to a buffer into which the data is written.
//   bytesToRead: Number of bytes that shall be read from the receive buffer.
//   bytesRead:   [out] Number of bytes actually read from the receive buffer.
// Returns:
//   Error code.
//   Possible errors: NODRIVER, ILLCLIENT, QRCVEMPTY, ILLPARAMVAL, RESOURCE.

can_status_t __stdcall CAN_Read(
        can_device_t device, 
        HCANCLIENT   client, 
        void         *buffer, 
        UINT32       bytesToRead,
        UINT32       *bytesRead);


//-----------------------------------------------------------------------------
// CAN_Write()
//   Writes a number of CAN messages or other commands into the transmit queue
//   of a client.
// Parameters:
//   device:       PCAN device to be used.
//   buffer:       Pointer to a buffer that contains the messages to write.
//   bytesToWrite: Number of bytes that shall be written.
//   bytesWritten: [out] Number of bytes actually written.
// Returns:
//   Error code.
//   Possible errors: NODRIVER, ILLCLIENT, ILLNET, BUSOFF, QXMTFULL, ILLPARAMVAL.

can_status_t __stdcall CAN_Write(
        can_device_t device, 
        void         *buffer, 
        UINT32       bytesToWrite,
        UINT32       *bytesWritten);


//-----------------------------------------------------------------------------
// CAN_ResetClient()
//   Resets the receive and transmit queues of a client.
// Parameters:
//   device: PCAN device to be used.
//   client: Handle of the client whose queues shall be reset.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLCLIENT.

can_status_t __stdcall CAN_ResetClient(
        can_device_t device,
        HCANCLIENT   client);


//-----------------------------------------------------------------------------
// CAN_ResetHardware()
//   Resets the hardware (CAN controller) and initializes the controller
//   with the last valid Bit rate and filter settings.
//   If a net is connected to a hardware:
//   Resets the CAN controller, flushes the transmit queue.
//   Affects the other clients that are connected to the same hardware via
//   a PCAN net.
// Parameters:
//   device: PCAN device to be used.
//   hw:     Handle of the hardware to reset.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLHW, REGTEST.

can_status_t __stdcall CAN_ResetHardware(
        can_device_t device,
        HCANHW       hw);


//-----------------------------------------------------------------------------
// CAN_GetParam()
//   Gets a driver/hardware/net/client/API parameter value.
// Parameters:
//   device: PCAN device to be used.
//   param:  [in, out] The parameter to get.
// Returns:
//   Error code.
//   Possible errors: NODRIVER, ILLCLIENT, ILLNET, ILLHW, ILLPARAMTYPE, ILLPARAMVAL.

can_status_t __stdcall CAN_GetParam(
        can_device_t    device,
        can_any_param_t *param);


//-----------------------------------------------------------------------------
// CAN_SetParam()
//   Sets a driver/hardware/net/client parameter value.
// Parameters:
//   device: PCAN device to be used.
//   param:  The parameter to be set.
// Returns:
//   Error code.
//   Possible errors: NODRIVER, ILLCLIENT, ILLNET, ILLHW, ILLPARAMTYPE,
//                    ILLPARAMVAL, ILLMODE.

can_status_t __stdcall CAN_SetParam(
        can_device_t    device, 
        can_any_param_t *param);


//-----------------------------------------------------------------------------
// CAN_GetErrText()
//   Converts a combination of error flags to text.
// Parameters:
//   error:    The error flags to be converted.
//   textBuff: Buffer for resulting error text.
//   buffSize: Size of text buffer in bytes.
// Returns:
//   Error code. Possible errors: ILLPARAMVAL.

can_status_t __stdcall CAN_GetErrText(
        can_status_t error,
        LPSTR        textBuff,
        UINT32       buffSize);


//-----------------------------------------------------------------------------
// CAN_BitrateToBitsPerSecond()
//   Calculates true bits-per-second values from a bit rate parameter record.
// Parameters:
//   bitrate:     Pointer to bit rate record.
//   nominal_bps: [out] Output of nominal bit rate in bits/s.
//   data_bps:    [out] Output of data bit rate in bits/s.
// Returns:
//   Error code. Possible errors: ILLPARAMVAL.

can_status_t __stdcall CAN_BitrateToBitsPerSecond(
        can_any_bitrate_t *bitrate,
        UINT32            *nominal_bps,
        UINT32            *data_bps);


//-----------------------------------------------------------------------------
// CAN_GetAvailableHardware()
//   Returns information about the CAN hardware channels currently available on
//   the computer.
// Parameters:
//   deviceFilter:  Filters the device for which hardware is searched. If
//                  pcan_unknown is specified, hardware channels of all
//                  installed devices are returned.
//   hardwareBuff:  Pointer to array of can_available_hardware_t records. Can
//                  be NULL.
//   maxHardware:   Number of hardwareBuff array elements. Can be 0 if
//                  hardwareBuff == NULL.
//   hardwareCount: [out] Returns the number of found hardware channels.
// Returns:
//   Error code. Possible errors: NODRIVER, ILLPARAMVAL.

can_status_t _stdcall CAN_GetAvailableHardware(
        can_device_t             deviceFilter,
        can_available_hardware_t *hardwareBuff,
        UINT32                   maxHardware,
        UINT32                   *hardwareCount);


#ifdef __cplusplus
}
#endif
