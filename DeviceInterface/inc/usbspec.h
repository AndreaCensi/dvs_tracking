/************************************************************************
 *
 *  Module:       usbspec.h
 *  Long name:    USB Specification 1.1 and 2.0
 *  Description:  
 *
 *  Runtime Env.: any
 *  Author(s):    Guenter Hildebrandt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _USBSPEC_H_
#define _USBSPEC_H_


// pack the following structures                                                     
#include <PSHPACK1.H>

// In Borland C++, we have to force byte alignment.
// This is necessary because there seems to be a problem with the
// implementations of pshpackX.h and poppack.h.
#ifdef __BORLANDC__
#pragma pack(1)
#endif

//
// descriptor types
//
#define USB_DEVICE_DESCRIPTOR_TYPE                    0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE             0x02
#define USB_STRING_DESCRIPTOR_TYPE                    0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE                 0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE                  0x05
#define USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE          0x06
#define USB_OTHER_SPEED_CONFIGURATION_DESCRIPTOR_TYPE 0x07
#define USB_INTERFACE_ASSOCIATION_TYPE								0x0B


#define USB_HID_DESCRIPTOR_TYPE                       0x21


//
// USB classes
//
#define USB_DEVICE_CLASS_RESERVED           0x00
#define USB_DEVICE_CLASS_AUDIO              0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS     0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE    0x03
#define USB_DEVICE_CLASS_MONITOR            0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE 0x05
#define USB_DEVICE_CLASS_POWER              0x06
#define USB_DEVICE_CLASS_PRINTER            0x07
#define USB_DEVICE_CLASS_STORAGE            0x08
#define USB_DEVICE_CLASS_HUB                0x09
#define USB_DEVICE_CLASS_DATA								0x0A
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC    0xFF


//
// USB endpoint types
//
#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03


//
// device descriptor
//
typedef struct _USB_DEVICE_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  USHORT  bcdUSB;
  UCHAR   bDeviceClass;
  UCHAR   bDeviceSubClass;
  UCHAR   bDeviceProtocol;
  UCHAR   bMaxPacketSize0;
  USHORT  idVendor;
  USHORT  idProduct;
  USHORT  bcdDevice;
  UCHAR   iManufacturer;
  UCHAR   iProduct;
  UCHAR   iSerialNumber;
  UCHAR   bNumConfigurations;
} USB_DEVICE_DESCRIPTOR;


//
// configuration descriptor
//
typedef struct _USB_CONFIGURATION_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  USHORT  wTotalLength;
  UCHAR   bNumInterfaces;
  UCHAR   bConfigurationValue;
  UCHAR   iConfiguration;
  UCHAR   bmAttributes;
  UCHAR   MaxPower;
} USB_CONFIGURATION_DESCRIPTOR;


//
// interface descriptor
//
typedef struct _USB_INTERFACE_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  UCHAR   bInterfaceNumber;
  UCHAR   bAlternateSetting;
  UCHAR   bNumEndpoints;
  UCHAR   bInterfaceClass;
  UCHAR   bInterfaceSubClass;
  UCHAR   bInterfaceProtocol;
  UCHAR   iInterface;
} USB_INTERFACE_DESCRIPTOR;


//
// endpoint descriptor
//
typedef struct _USB_ENDPOINT_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  UCHAR   bEndpointAddress;
  UCHAR   bmAttributes;
  USHORT  wMaxPacketSize;
  UCHAR   bInterval;
} USB_ENDPOINT_DESCRIPTOR;


//
// string descriptor
//
typedef struct _USB_STRING_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  WCHAR   bString[1]; // variable size
} USB_STRING_DESCRIPTOR;

//
// device qualifier descriptor
//
typedef struct _USB_DEVICE_QUALIFIER_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  USHORT  bcdUSB;
  UCHAR   bDeviceClass;
  UCHAR   bDeviceSubClass;
  UCHAR   bDeviceProtocol;
  UCHAR   bMaxPacketSize0;
  UCHAR   bNumConfigurations;
  UCHAR   bReserved;
} USB_DEVICE_QUALIFIER_DESCRIPTOR;

//
// interface association descriptor
//
typedef struct _USB_INTERFACE_ASSOCIATION_DESCRIPTOR {
	UCHAR bLength;
	UCHAR bDescriptorType;
	UCHAR bFirstInterface;
	UCHAR bInterfaceCount;
	UCHAR bFunctionClass;
	UCHAR bFunctionSubClass;
	UCHAR bFunctionProtocol;
	UCHAR iFunction;
} USB_INTERFACE_ASSOCIATION_DESCRIPTOR;


//
// HID descriptor
//
typedef struct _USB_HID_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  USHORT  bcdHID; 
  UCHAR   bCountryCode;
  UCHAR   bNumDescriptors;
  UCHAR   bDescriptorType1;
  USHORT  wDescriptorLength1; 
} USB_HID_DESCRIPTOR;


//
// common descriptor header
//
typedef struct _USB_COMMON_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
} USB_COMMON_DESCRIPTOR;




// **********************
// * Audio Device Class *
// **********************
// interface descriptor class code 0x01

//
// Audio specific descriptor types
//

#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE					USB_ENDPOINT_DESCRIPTOR_TYPE
#define AUDIO_CS_UNDEFINED_DESCRIPTOR_TYPE			0x20
#define AUDIO_CS_DEVICE_DESCRIPTOR_TYPE					0x21
#define AUDIO_CS_CONFIGURATION_DESCRIPTOR_TYPE	0x22
#define AUDIO_CS_STRING_DESCRIPTOR_TYPE					0x23
#define AUDIO_CS_INTERFACE_DESCRIPTOR_TYPE			0x24
#define AUDIO_CS_ENDPOINT_DESCRIPTOR_TYPE				0x25

//
// Audio specific descriptor sub types
//
#define AUDIO_CS_SUBTYPE_HEADER		          0x01
#define AUDIO_CS_SUBTYPE_INPUT_TERMINAL			0x02
#define AUDIO_CS_SUBTYPE_OUTPUT_TERMINAL		0x03
#define AUDIO_CS_SUBTYPE_MIXER_UNIT		      0x04
#define AUDIO_CS_SUBTYPE_SELECTOR_UNIT			0x05
#define AUDIO_CS_SUBTYPE_FEATUR_UNIT				0x06
#define AUDIO_CS_SUBTYPE_PROCESSING_UNIT		0x07
#define AUDIO_CS_SUBTYPE_EXTENSION_UNIT			0x08

//
// common audio descriptor header
//
typedef struct _AUDIO_COMMON_INTERFACE_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  UCHAR   bDescriptorSubtypeType;
} AUDIO_COMMON_INTERFACE_DESCRIPTOR;

//
// Header
//
typedef struct _AUDIO_INTERFACE_HEADER_DESCRIPTOR {
  USHORT  bcdADC;
  USHORT  bTotalLength;
  UCHAR   blnCollection;
  // UCHAR baInterfaceNr[];
} AUDIO_INTERFACE_HEADER_DESCRIPTOR;

//
// Input Terminal
//
typedef struct _AUDIO_INTERFACE_INPUT_TERMINAL_DESCRIPTOR {
  UCHAR   bTerminalId;
  USHORT  wTerminalType;
  UCHAR   bAssocTerminal;
  UCHAR   bNrChannels;
  USHORT  wChannelConfig;
  UCHAR   iChannelNames;
  UCHAR   iTerminal;
} AUDIO_INTERFACE_INPUT_TERMINAL_DESCRIPTOR;

//
// Output Terminal
//
typedef struct _AUDIO_INTERFACE_OUTPUT_TERMINAL_DESCRIPTOR {
  UCHAR   bTerminalId;
  USHORT  wTerminalType;
  UCHAR   bAssocTerminal;
  UCHAR   bSourceID;
  UCHAR   iTerminal;
} AUDIO_INTERFACE_OUTPUT_TERMINAL_DESCRIPTOR;

//
// Feature Unit
//
typedef struct _AUDIO_INTERFACE_FEATURE_UNIT_DESCRIPTOR {
  UCHAR   bUnitID;
  UCHAR   bSourceID;
  UCHAR   bControlSize;
  // UCHAR    bmaControls[];
	// UCHAR		iFeature;
} AUDIO_INTERFACE_FEATURE_UNIT_DESCRIPTOR;

//
// AS General
//
typedef struct _AUDIO_INTERFACE_AS_GENERAL_DESCRIPTOR {
  UCHAR   bTerminalLink;
  UCHAR   bDelay;
  USHORT  wFormatTag;
} AUDIO_INTERFACE_AS_GENERAL_DESCRIPTOR;

//
// AS Format Type 1
//
typedef struct _AUDIO_FORMAT_TYPE_I_DESCRIPTOR {
  UCHAR   bFormatType;
  UCHAR   bNrChannels;
  UCHAR   bSubframeSize;
  UCHAR   bBitResolution;
  UCHAR   bSamFreqType;
} AUDIO_FORMAT_TYPE_I_DESCRIPTOR;



//
// Audio endpoint descriptor
//
typedef struct _AUDIO_ENDPOINT_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  UCHAR   bEndpointAddress;
  UCHAR   bmAttributes;
  USHORT  wMaxPacketSize;
  UCHAR   bInterval;
  UCHAR   bRefresh;
  UCHAR   bSynchAddress;
} AUDIO_ENDPOINT_DESCRIPTOR;


//
// Audio CS Endpoint descriptor
//

typedef struct _AUDIO_CS_ENDPOINT_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  UCHAR   bDescriptorSubtypeType;
  UCHAR   bmAttributes;   
  UCHAR   bLockDelayUnits;    
  USHORT  wLockDelay;   
} AUDIO_CS_ENDPOINT_DESCRIPTOR;


// ************************************
// * Communication Device Class (CDC) *
// ************************************
// device descriptor class code				0x02
// interface descriptor class code		0x02
#define USB_IF_CLASS_COMMUNICATIONS		0x02
#define USB_IF_CLASS_DATA							0x0A

//
// CDC specific descriptor types
//
#define CDC_CS_INTERFACE_DESCRIPTOR_TYPE						0x24
#define CDC_CS_ENDPOINT_DESCRIPTOR_TYPE							0x25

//
// CDC Functional Descriptor SubType Codes
//
#define CDC_FDST_HEADER															0x00
#define CDC_FDST_CALL_MANAGEMENT										0x01
#define CDC_FDST_ABSTRACT_CONTROL_MANAGEMENT				0x02
#define CDC_FDST_DIRECT_LINE_MANAGEMENT							0x03
#define CDC_FDST_TELEPHONE_RINGER										0x04
#define CDC_FDST_TELEPHONE_CALL_AND_STATE_REPORT		0x05
#define CDC_FDST_UNION															0x06
#define CDC_FDST_COUNTRY_SELECTION									0x07
#define CDC_FDST_TELEPHONE_OPERATIONAL_MODES				0x08
#define CDC_FDST_USB_TERMINAL												0x09
#define CDC_FDST_NETWORK_CHANNEL_TERMINAL						0x0A
#define CDC_FDST_PROTOCOL_UNIT											0x0B
#define CDC_FDST_EXTENSION_UNIT											0x0C
#define CDC_FDST_MULTI_CHANNEL_MANAGEMENT						0x0D
#define CDC_FDST_CAPI_CONTROL_MANAGEMENT						0x0E
#define CDC_FDST_ETHERNET_NETWORKING								0x0F
#define CDC_FDST_ATM_NETWORKING											0x10
#define CDC_FDST_WIRELESS_HANDSET										0x11		// WHCM functional descriptor
#define CDC_FDST_MOBILE_DIRECT_LINE									0x12
#define CDC_FDST_MDLM_DETAIL												0x13
#define CDC_FDST_DEVICE_MANAGMENT_MODEL							0x14
#define CDC_FDST_OBEX																0x15
#define CDC_FDST_COMMAND_SET												0x16
#define CDC_FDST_COMMAND_SET_DETAIL									0x17
#define CDC_FDST_TELEPHONE_CONTROL_MODEL						0x18
// 0x19 - 0xFF reserved for future use

//
// CDC Communication Interface Class SubClass Codes
//
#define CDC_ISC_RESERVED														0x00
#define CDC_ISC_DIRECT_LINE_CONTROL_MODEL						0x01
#define CDC_ISC_ABSTRACT_CONTROL_MODEL							0x02
#define CDC_ISC_TELEPHONE_CONTROL_MODEL							0x03
#define CDC_ISC_MULTI_CHANNEL_CONTROL_MODEL					0x04
#define CDC_ISC_CAPI_CONTROL_MODEL									0x05
#define CDC_ISC_ETHERNET_NETWORKING_CONTROL_MODEL		0x06
#define CDC_ISC_ATM_NETWORKING_CONTROL_MODEL				0x07
#define CDC_ISC_WIRELESS_HANDSET_CONTROL_MODEL			0x08
#define CDC_ISC_DEVICE_MANAGMENT_MODEL							0x09
#define CDC_ISC_MOBILE_DIRECT_LINE_MODEL						0x0A
#define CDC_ISC_OBEX_MODEL													0x0B
// 0x0C - 0x7F reserved for future use
// 0x80 - 0xFE reserved for vendor specific definition


//
// CDC Communication Interface Class Protocol Codes
//
#define CDC_CIP_NO_CLASS_SPECIFIC_PROTOCOL					0x00
#define CDC_CIP_AT_HAYES_COMPATIBLE									0x01
#define CDC_CIP_AT_PCCA101													0x02
#define CDC_CIP_AT_PCCA101_O												0x03
#define CDC_CIP_AT_GSM07_07													0x04
#define CDC_CIP_AT_3GPP27_007												0x05
#define CDC_CIP_AT_C_S0017_0												0x06
// 0x07 - 0xFD reserved for future use
#define CDC_CIP_EXTERNAL_PROTOCOL										0xFE
#define CDC_CIP_VENDOR_SPECIFIC											0xFF


//
// CDC Data Interface Class SubClass Codes
//
// this field is unused at time


//
// CDC Data Interface Class Protocol Codes
//
#define CDC_DIP_NO_CLASS_SPECIFIC_PROTOCOL					0x00
// 0x01 - 0x2F reserved for future use
#define CDC_DIP_ISDN_BRI														0x30
#define CDC_DIP_HDLC																0x31
#define CDC_DIP_TRANSPARENT													0x32
// 0x33 - 0x4F reserved for future use
#define CDC_DIP_MANAGMENT_PROTOCOL_Q921							0x50
#define CDC_DIP_DATA_LINK_PROTOCOL_Q921							0x51
#define CDC_DIP_TEI_MULTIPLEXOR_FOR_Q921						0x52
// 0x53 - 0x8F reserved for future use
#define CDC_DIP_V42_DATA_COMPRESSION								0x90
#define CDC_DIP_EURO_ISDN														0x91
#define CDC_DIP_V24_RATE_ADAPTATION_TO_ISDN					0x92
#define CDC_DIP_CAPI20															0x93
// 0x94 - 0xFC reserved for future use
#define CDC_DIP_HOST_BASED_DRIVER										0xFD
#define CDC_DIP_CDC_PROTOCOL_UNIT_FUNC_DESC					0xFE
#define CDC_DIP_VENDOR_SPECIFIC											0xFF



//
// Common CDC descriptor header
//
typedef struct _CDC_COMMON_INTERFACE_DESCRIPTOR {
  UCHAR   bLength;
  UCHAR   bDescriptorType;
  UCHAR   bDescriptorSubtypeType;
} CDC_COMMON_INTERFACE_DESCRIPTOR;


//
// Header Functional Descriptor		0x00
//
typedef struct _CDC_HEADER_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_HEADER
  USHORT  bcdCDC;												// USB CDC specification release number, binary coded decimal
} CDC_HEADER_FUNC_DESC;


//
// Call Management Functional Descriptor		0x01
//
typedef struct _CDC_CALL_MANAGEMENT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_CALL_MANAGEMENT
  UCHAR		bmCapabilities;								// supported capabilities, binary flag field
  UCHAR		bDataInterface;								// interface number of data class interface, zero based index
} CDC_CALL_MANAGEMENT_FUNC_DESC;
#define CDC_CM_FLAG_HANDLE_ITSELF										0x01
#define CDC_CM_FLAG_SEND_RCV_OVER_DATA_CLASS_IF			0x02
// bit 2..7 reserved


//
// Abstract Control Management Functional Descriptor		0x02
//
typedef struct _CDC_ABSTRACT_CONTROL_MANAGEMENT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_ABSTRACT_CONTROL_MANAGEMENT
  UCHAR		bmCapabilities;								// supported capabilities, binary flag field
} CDC_ABSTRACT_CONTROL_MANAGEMENT_FUNC_DESC;
#define CDC_ACM_FLAG_SUPPORT_COMM_FEATURES					0x01
#define CDC_ACM_FLAG_SUPPORT_LINE_CODING_AND_STATE	0x02
#define CDC_ACM_FLAG_SUPPORT_SEND_BREAK							0x04
#define CDC_ACM_FLAG_SUPPORT_NETWORK_CONNECTION			0x08
// bit 4..7 reserved


//
// Direct Line Management Functional Descriptor		0x03
//
typedef struct _CDC_DIRECT_LINE_MANAGEMENT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_DIRECT_LINE_MANAGEMENT
  UCHAR		bmCapabilities;								// supported capabilities, binary flag field
} CDC_DIRECT_LINE_MANAGEMENT_FUNC_DESC;
#define CDC_DLM_FLAG_SUPPORT_PULSE									0x01
#define CDC_DLM_FLAG_SUPPORT_AUX										0x02
#define CDC_DLM_FLAG_REQUIRE_EXTRA_PULSE						0x04
// bit 3..7 reserved


//
// Telephone Ringer Functional Descriptor		0x04
//
typedef struct _CDC_TELEPHONE_RINGER_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_TELEPHONE_RINGER
  UCHAR		bRingerVolumeSteps;						// number of supported discrete volume steps 1..255, 0=256
	UCHAR		bNumRingerPatterns;						// number of supported ringer patterns 1..255, 0-reserved
} CDC_TELEPHONE_RINGER_FUNC_DESC;


//
// Telephone Call and Line State Reporting Capabilities Descriptor		0x05
//
typedef struct _CDC_CALL_AND_LINE_STATE_REPORT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_TELEPHONE_CALL_AND_STATE_REPORT
  ULONG		bmCapabilities;								// supported capabilities, binary flag field
} CDC_CALL_AND_LINE_STATE_REPORT_FUNC_DESC;
#define CDC_CLSR_FLAG_REPORT_INTERRUPTED_DIALTONE		0x00000001
#define CDC_CLSR_FLAG_REPORT_RINGBACK_BUSY					0x00000002
#define CDC_CLSR_FLAG_REPORT_CALLER_ID							0x00000004
#define CDC_CLSR_FLAG_REPORT_RINGING_PATTERN				0x00000008
#define CDC_CLSR_FLAG_REPORT_DTMF_DIGITS						0x00000010
#define CDC_CLSR_FLAG_SUPPORT_LINE_STATE_CHANGE			0x00000020
// bit 6..31 reserved


//
// Union Functional Descriptor		0x06
//
typedef struct _CDC_UNION_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_UNION
  UCHAR		bMasterIf;										// number of interface designated as master or controlling interface
  UCHAR		bSlaveIf[1];									// variable field depending on bLength, 
																				//		interface numbers of slaves or associated interfaces
} CDC_UNION_FUNC_DESC;


//
// Country Selection Functional Descriptor		0x07
//
typedef struct _CDC_COUNTRY_SELECTION_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_COUNTRY_SELECTION
  UCHAR		iCountryCodeRelDate;					// index of string giving release date for 
																				//		implemented ISO3166 country codes, format: ddmmyyyy
  USHORT	wCountryCode[1];							// variable field depending on bLength, 
																				//		country codes in hexadecimal format as defined in ISO3166
} CDC_COUNTRY_SELECTION_FUNC_DESC;


//
// Telephone Operational Modes Functional Descriptor		0x08
//
typedef struct _CDC_TELEPHONE_OPERATIONAL_MODES_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_TELEPHONE_OPERATIONAL_MODES
  UCHAR		bmCapabilities;								// supported operational modes, binary flag field
} CDC_TELEPHONE_OPERATIONAL_MODES_FUNC_DESC;
#define CDC_TOM_FLAG_SUPPORT_SIMPLE_MODE						0x01
#define CDC_TOM_FLAG_SUPPORT_STANDALONE_MODE				0x02
#define CDC_TOM_FLAG_SUPPORT_COMPUTER_CENTRIC_MODE	0x04
// bit 3..7 reserved


//
// USB Terminal Functional Descriptor		0x09
//
typedef struct _CDC_USB_TERMINAL_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_USB_TERMINAL
  UCHAR   bEntityId;										// constant uniquely identifying the terminal
  UCHAR		bInInterfaceNo;								// the input interface number of associated USB interface
  UCHAR		bOutInterfaceNo;							// the output interface number of associated USB interface
  UCHAR		bmOptions;										// options, binary flag field
  UCHAR		bChildId[1];									// variable field depending on bLength, 
																				//		id of connected lower terminal or unit
} CDC_USB_TERMINAL_FUNC_DESC;
#define CDC_USBT_FLAG_WRAPPER_USED									0x01
// bit 1..7 reserved


//
// Network Channel Terminal Functional Descriptor		0x0A
//
typedef struct _CDC_NW_CHANNEL_TERMINAL_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_NETWORK_CHANNEL_TERMINAL
  UCHAR   bEntityId;										// constant uniquely identifying the terminal
  UCHAR		iName;												// index of string descriptor describing the name of the terminal
  UCHAR		bChannelIdx;									// channel index of associated network channel
  UCHAR		bPhysicalIf;									// type of physical interface
} CDC_NW_CHANNEL_TERMINAL_FUNC_DESC;
#define CDC_NW_CT_NONE															0x00
#define CDC_NW_CT_ISDN															0x01
// 2..200 reserved for future use
// 201..255 reserved for vendor specific definition


//
// Protocol Unit Functional Descriptor		0x0B
//
typedef struct _CDC_PROTOCOL_UNIT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_PROTOCOL_UNIT
  UCHAR   bEntityId;										// constant uniquely identifying the terminal
  UCHAR		bProtocol;										// protocol code, 
																				//		defined as CDC Data Interface Class Protocol Codes (CDC_DIP_)
  UCHAR		bChildId[1];									// variable field depending on bLength, 
																				//		id of connected lower terminal or unit
} CDC_PROTOCOL_UNIT_FUNC_DESC;


//
// Extension Unit Functional Descriptor		0x0C
//
typedef struct _CDC_EXTENSION_UNIT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_EXTENSION_UNIT
  UCHAR   bEntityId;										// constant uniquely identifying the terminal
  UCHAR   bExtensionCode;								// vendor specific code to identify the extension unit
  UCHAR		iName;												// index of string descriptor describing the name of the extension unit
  UCHAR		bChildId[1];									// variable field depending on bLength, 
																				//		id of connected lower terminal or unit
} CDC_EXTENSION_UNIT_FUNC_DESC;


//
// Multi-Channel Management Functional Descriptor		0x0D
//
typedef struct _CDC_MULTI_CHANNEL_MANAGEMENT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_MULTI_CHANNEL_MANAGEMENT
  UCHAR		bmCapabilities;								// supported capabilities, binary flag field
} CDC_MULTI_CHANNEL_MANAGEMENT_FUNC_DESC;
#define CDC_MCM_FLAG_SUPPORT_NON_VOLATILE_MEMORY		0x01
#define CDC_MCM_FLAG_SUPPORT_CLEAR_UNIT_PARAMS			0x02
#define CDC_MCM_FLAG_SUPPORT_SET_UNIT_PARAMS				0x04
// bit 3..7 reserved


//
// CAPI Control Management Functional Descriptor		0x0E
//
typedef struct _CDC_CAPI_CONTROL_MANAGEMENT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_CAPI_CONTROL_MANAGEMÈNT
  UCHAR		bmCapabilities;								// supported capabilities, binary flag field
} CDC_CAPI_CONTROL_MANAGEMENT_FUNC_DESC;
#define CDC_CCM_FLAG_IS_INTELLIGENT_CAPI_DEVICE			0x01
// bit 1..7 reserved


//
// Ethernet Networking Functional Descriptor		0x0F
//
typedef struct _CDC_ETHERNET_NETWORKING_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_ETHERNET_NETWORKING
  UCHAR		iMACAddress;									// index of string descriptor holding the 48bit MAC address
  ULONG		bmEthernetStatistics;					// supported statistics functions, binary flag field
  USHORT	wMaxSegmentSize;							// maximum supported segment size
  USHORT	wNumberMultiCastFilters;			// number of configurable multi cast filters
  UCHAR   bNumberPowerFilters;					// number of available pattern filters
} CDC_ETHERNET_NETWORKING_FUNC_DESC;
#define CDC_ENW_SC_XMIT_OK													0x00000001
#define CDC_ENW_SC_RCV_OK														0x00000002
#define CDC_ENW_SC_XMIT_ERROR												0x00000004
#define CDC_ENW_SC_RCV_ERROR												0x00000008
#define CDC_ENW_SC_RCV_NO_BUFFER										0x00000010
#define CDC_ENW_SC_DIRECTED_BYTES_XMIT							0x00000020
#define CDC_ENW_SC_DIRECTED_FRAMES_XMIT							0x00000040
#define CDC_ENW_SC_MULTICAST_BYTES_XMIT							0x00000080
#define CDC_ENW_SC_MULTICAST_FRAMES_XMIT						0x00000100
#define CDC_ENW_SC_BROADCAST_BYTES_XMIT							0x00000200
#define CDC_ENW_SC_BROADCAST_FRAMES_XMIT						0x00000400
#define CDC_ENW_SC_DIRECTED_BYTES_RCV								0x00000800
#define CDC_ENW_SC_DIRECTED_FRAMES_RCV							0x00001000
#define CDC_ENW_SC_MULTICAST_BYTES_RCV							0x00002000
#define CDC_ENW_SC_MULTICAST_FRAMES_RCV							0x00004000
#define CDC_ENW_SC_BROADCAST_BYTES_RCV							0x00008000
#define CDC_ENW_SC_BROADCAST_FRAMES_RCV							0x00010000
#define CDC_ENW_SC_RCV_CRC_ERROR										0x00020000
#define CDC_ENW_SC_TRANSMIT_QUEUE_LENGTH						0x00040000
#define CDC_ENW_SC_RCV_ERROR_ALIGNMENT							0x00080000
#define CDC_ENW_SC_XMIT_ONE_COLLISION								0x00100000
#define CDC_ENW_SC_XMIT_MORE_COLLISIONS							0x00200000
#define CDC_ENW_SC_XMIT_DEFERRED										0x00400000
#define CDC_ENW_SC_XMIT_MAX_COLLISIONS							0x00800000
#define CDC_ENW_SC_RCV_OVERRUN											0x01000000
#define CDC_ENW_SC_XMIT_UNDERRUN										0x02000000
#define CDC_ENW_SC_XMIT_HEARTBEAT_FAILURE						0x04000000
#define CDC_ENW_SC_XMIT_TIMES_CRS_LOST							0x08000000
#define CDC_ENW_SC_XMIT_LATE_COLLISIONS							0x10000000
// bit 29..31 reserved


//
// ATM Networking Functional Descriptor		0x10
//
typedef struct _CDC_ATM_NETWORKING_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_ATM_NETWORKING
  UCHAR		iEndSystemId;									// index of string descriptor holding the End System Identifier
  UCHAR		bmDataCapabilities;						// supported ATM data types, binary flag field (CDC_ANW_DC_)
  UCHAR		bmATMDeviceStatistics;				// supported optional statistics functions, 
																				//		binary flag field (CDC_ANW_DS_)
  USHORT	wType2MaxSegmentSize;					// maximum segment size that the type2 device is capable of supporting
  USHORT	wType3MaxSegmentSize;					// maximum segment size that the type3 device is capable of supporting
  USHORT	wMaxVC;												// maximum number of supported simultaneous virtual circuits, type3 only
} CDC_ATM_NETWORKING_FUNC_DESC;
#define CDC_ANW_DC_RESERVED													0x01
#define CDC_ANW_DC_TYPE1														0x02
#define CDC_ANW_DC_TYPE2														0x04
#define CDC_ANW_DC_TYPE3														0x08
// bit 4..7 reserved
#define CDC_ANW_DS_DOWNSTREAM_CELLS_RCV							0x01
#define CDC_ANW_DS_UPSTREAM_CELLS_SENT							0x02
#define CDC_ANW_DS_CELLS_HEC_ERROR_CORRECTED				0x04
#define CDC_ANW_DS_VC_DOWNSTREAM_CELLS_RCV					0x08
#define CDC_ANW_DS_VC_UPSTREAM_CELLS_SENT						0x10
// bit 5..7 reserved


//
// WHCM (Wireless Handset) functional descriptor		0x11
//
typedef struct _CDC_WIRELESS_HANDSET_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_WIRELESS_HANDSET
  USHORT  bcdVersion;										// version number of subclass specification, Mj.Mn
} CDC_WIRELESS_HANDSET_FUNC_DESC;


//
// Mobile Direct Line Model Functional Descriptor		0x12
//
typedef struct _CDC_MOBIL_DIRECT_LINE_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_MOBILE_DIRECT_LINE
  USHORT  bcdVersion;										// version number of subclass specification, Mj.Mn
  UCHAR		bGUID[16];										// specifies the detailed transport protocol of the interface (LEACH1998)
} CDC_MOBIL_DIRECT_LINE_FUNC_DESC;


//
// Mobile Direct Line Model Detail Functional Descriptor		0x13
//
typedef struct _CDC_MDLM_DETAIL_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_MDLM_DETAIL
  UCHAR		bGuidDescriptorType;					// discriminator, 
																				//		interpretation depend on GUID in CDC_MOBIL_DIRECT_LINE_FUNC_DESC 
  UCHAR		bDetailData[1];								// variable field depending on bLength, information associated with
																				//		the GUID in CDC_MOBIL_DIRECT_LINE_FUNC_DESC
} CDC_MDLM_DETAIL_FUNC_DESC;


//
// Device Management Functional Descriptor		0x14
//
typedef struct _CDC_DEVICE_MANAGEMENT_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_DEVICE_MANAGMENT_MODEL
  USHORT  bcdVersion;										// version number of subclass specification, Mj.Mn
  USHORT  wMaxCommand;									// maximum buffer size allocated in the device for data sent
																				//		from the host using SEND_ENCAPSULATED_COMMAND
} CDC_DEVICE_MANAGEMENT_FUNC_DESC;


//
// OBEX Control Model Functional Descriptor		0x15
//
typedef struct _CDC_OBEX_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_OBEX
  USHORT  bcdVersion;										// version number of subclass specification, Mj.Mn
} CDC_OBEX_FUNC_DESC;


//
// Command Set Functional Descriptor		0x16
//
typedef struct _CDC_COMMAND_SET_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_COMMAND_SET
  USHORT  bcdVersion;										// version number of subclass specification, Mj.Mn
  UCHAR   iCommandSet;									// index of string descriptor describing the command set
  UCHAR		bGUID[16];										// specifies the detailed transport protocol of the interface (LEACH1998)
} CDC_COMMAND_SET_FUNC_DESC;


//
// Command Set Detail Functional Descriptor		0x17
//
typedef struct _CDC_COMMAND_SET_DETAIL_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_COMMAND_SET_DETAIL
  UCHAR		bGuidDescriptorType;					// discriminator, 
																				//		interpretation depend on GUID in CDC_MOBIL_DIRECT_LINE_FUNC_DESC 
  UCHAR		bDetailData[1];								// variable field depending on bLength, information associated with
																				//		the GUID in CDC_MOBIL_DIRECT_LINE_FUNC_DESC
} CDC_COMMAND_SET_DETAIL_FUNC_DESC;


//
// Telephone Control Model Functional Descriptor		0x18
//
typedef struct _CDC_TELEPHONE_CONTROL_MODEL_FUNC_DESC {
  UCHAR   bLength;
  UCHAR   bDescriptorType;							// constant, CDC_CS_INTERFACE_DESCRIPTOR_TYPE
  UCHAR   bDescriptorSubType;						// constant, CDC_FDST_TELEPHONE_CONTROL_MODEL
  USHORT  bcdVersion;										// version number of subclass specification, Mj.Mn
  USHORT  wMaxCommand;									// maximum number of characters 
																				//		may be transported in a SEND_ENCAPSULATED_COMMAND
} CDC_TELEPHONE_CONTROL_MODEL_FUNC_DESC;


// restore previous settings
#ifdef __BORLANDC__
#pragma pack()
#endif

// restore packing
#include <POPPACK.H>


#endif  // _USBSPEC_H_

/*************************** EOF **************************************/
