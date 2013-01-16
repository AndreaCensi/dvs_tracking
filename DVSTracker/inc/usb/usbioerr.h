/************************************************************************
 *
 *  Module:       usbioerr.h
 *  Long name:    USBIO Driver status codes
 *  Description:  Defines the status codes of the USBIO driver
 *
 *  Runtime Env.: Win32, kernel mode
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _USBIOERR_H_
#define _USBIOERR_H_
//
// USBIO Error Codes
//
#define USBIO_ERR_SUCCESS                              0x00000000L
#define USBIO_ERR_CRC                                  0xE0000001L
#define USBIO_ERR_BTSTUFF                              0xE0000002L
#define USBIO_ERR_DATA_TOGGLE_MISMATCH                 0xE0000003L
#define USBIO_ERR_STALL_PID                            0xE0000004L
#define USBIO_ERR_DEV_NOT_RESPONDING                   0xE0000005L
#define USBIO_ERR_PID_CHECK_FAILURE                    0xE0000006L
#define USBIO_ERR_UNEXPECTED_PID                       0xE0000007L
#define USBIO_ERR_DATA_OVERRUN                         0xE0000008L
#define USBIO_ERR_DATA_UNDERRUN                        0xE0000009L
#define USBIO_ERR_RESERVED1                            0xE000000AL
#define USBIO_ERR_RESERVED2                            0xE000000BL
#define USBIO_ERR_BUFFER_OVERRUN                       0xE000000CL
#define USBIO_ERR_BUFFER_UNDERRUN                      0xE000000DL
#define USBIO_ERR_NOT_ACCESSED                         0xE000000FL
#define USBIO_ERR_FIFO                                 0xE0000010L
#define USBIO_ERR_XACT_ERROR                           0xE0000011L
#define USBIO_ERR_BABBLE_DETECTED                      0xE0000012L
#define USBIO_ERR_DATA_BUFFER_ERROR                    0xE0000013L

#define USBIO_ERR_ENDPOINT_HALTED                      0xE0000030L
#define USBIO_ERR_NO_MEMORY                            0xE0000100L
#define USBIO_ERR_INVALID_URB_FUNCTION                 0xE0000200L
#define USBIO_ERR_INVALID_PARAMETER                    0xE0000300L
#define USBIO_ERR_ERROR_BUSY                           0xE0000400L
#define USBIO_ERR_REQUEST_FAILED                       0xE0000500L
#define USBIO_ERR_INVALID_PIPE_HANDLE                  0xE0000600L
#define USBIO_ERR_NO_BANDWIDTH                         0xE0000700L
#define USBIO_ERR_INTERNAL_HC_ERROR                    0xE0000800L
#define USBIO_ERR_ERROR_SHORT_TRANSFER                 0xE0000900L
#define USBIO_ERR_BAD_START_FRAME                      0xE0000A00L
#define USBIO_ERR_ISOCH_REQUEST_FAILED                 0xE0000B00L
#define USBIO_ERR_FRAME_CONTROL_OWNED                  0xE0000C00L
#define USBIO_ERR_FRAME_CONTROL_NOT_OWNED              0xE0000D00L
#define USBIO_ERR_NOT_SUPPORTED                        0xE0000E00L
#define USBIO_ERR_INVALID_CONFIGURATION_DESCRIPTOR     0xE0000F00L

#define USBIO_ERR_INSUFFICIENT_RESOURCES               0xE8001000L
#define USBIO_ERR_SET_CONFIG_FAILED                    0xE0002000L
#define USBIO_ERR_USBD_BUFFER_TOO_SMALL                0xE0003000L
#define USBIO_ERR_USBD_INTERFACE_NOT_FOUND             0xE0004000L
#define USBIO_ERR_INVALID_PIPE_FLAGS                   0xE0005000L
#define USBIO_ERR_USBD_TIMEOUT                         0xE0006000L
#define USBIO_ERR_DEVICE_GONE                          0xE0007000L
#define USBIO_ERR_STATUS_NOT_MAPPED                    0xE0008000L

#define USBIO_ERR_CANCELED                             0xE0010000L
#define USBIO_ERR_ISO_NOT_ACCESSED_BY_HW               0xE0020000L
#define USBIO_ERR_ISO_TD_ERROR                         0xE0030000L
#define USBIO_ERR_ISO_NA_LATE_USBPORT                  0xE0040000L
#define USBIO_ERR_ISO_NOT_ACCESSED_LATE                0xE0050000L

#define USBIO_ERR_FAILED                               0xE0001000L
#define USBIO_ERR_INVALID_INBUFFER                     0xE0001001L
#define USBIO_ERR_INVALID_OUTBUFFER                    0xE0001002L
#define USBIO_ERR_OUT_OF_MEMORY                        0xE0001003L
#define USBIO_ERR_PENDING_REQUESTS                     0xE0001004L
#define USBIO_ERR_ALREADY_CONFIGURED                   0xE0001005L
#define USBIO_ERR_NOT_CONFIGURED                       0xE0001006L
#define USBIO_ERR_OPEN_PIPES                           0xE0001007L
#define USBIO_ERR_ALREADY_BOUND                        0xE0001008L
#define USBIO_ERR_NOT_BOUND                            0xE0001009L
#define USBIO_ERR_DEVICE_NOT_PRESENT                   0xE000100AL
#define USBIO_ERR_CONTROL_NOT_SUPPORTED                0xE000100BL
#define USBIO_ERR_TIMEOUT                              0xE000100CL
#define USBIO_ERR_INVALID_RECIPIENT                    0xE000100DL
#define USBIO_ERR_INVALID_TYPE                         0xE000100EL
#define USBIO_ERR_INVALID_IOCTL                        0xE000100FL
#define USBIO_ERR_INVALID_DIRECTION                    0xE0001010L
#define USBIO_ERR_TOO_MUCH_ISO_PACKETS                 0xE0001011L
#define USBIO_ERR_POOL_EMPTY                           0xE0001012L
#define USBIO_ERR_PIPE_NOT_FOUND                       0xE0001013L
#define USBIO_ERR_INVALID_ISO_PACKET                   0xE0001014L
#define USBIO_ERR_OUT_OF_ADDRESS_SPACE                 0xE0001015L
#define USBIO_ERR_INTERFACE_NOT_FOUND                  0xE0001016L
#define USBIO_ERR_INVALID_DEVICE_STATE                 0xE0001017L
#define USBIO_ERR_INVALID_PARAM                        0xE0001018L
#define USBIO_ERR_DEMO_EXPIRED                         0xE0001019L
#define USBIO_ERR_INVALID_POWER_STATE                  0xE000101AL
#define USBIO_ERR_POWER_DOWN                           0xE000101BL
#define USBIO_ERR_VERSION_MISMATCH                     0xE000101CL
#define USBIO_ERR_SET_CONFIGURATION_FAILED             0xE000101DL
#define USBIO_ERR_ADDITIONAL_EVENT_SIGNALLED           0xE000101EL

#define USBIO_ERR_INVALID_PROCESS                      0xE000101FL

#define USBIO_ERR_DEVICE_ACQUIRED                      0xE0001020L

#define USBIO_ERR_DEVICE_OPENED                        0xE0001021L

#define USBIO_ERR_VID_RESTRICTION                      0xE0001080L
#define USBIO_ERR_ISO_RESTRICTION                      0xE0001081L
#define USBIO_ERR_BULK_RESTRICTION                     0xE0001082L
#define USBIO_ERR_EP0_RESTRICTION                      0xE0001083L
#define USBIO_ERR_PIPE_RESTRICTION                     0xE0001084L
#define USBIO_ERR_PIPE_SIZE_RESTRICTION                0xE0001085L
#define USBIO_ERR_CONTROL_RESTRICTION                  0xE0001086L
#define USBIO_ERR_INTERRUPT_RESTRICTION                0xE0001087L

#define USBIO_ERR_DEVICE_NOT_FOUND                     0xE0001100L
#define USBIO_ERR_DEVICE_NOT_OPEN                      0xE0001102L
#define USBIO_ERR_NO_SUCH_DEVICE_INSTANCE              0xE0001104L
#define USBIO_ERR_INVALID_FUNCTION_PARAM               0xE0001105L
#define USBIO_ERR_LOAD_SETUP_API_FAILED                0xE0001106L
#define USBIO_ERR_DEVICE_ALREADY_OPENED                0xE0001107L

#define USBIO_ERR_INVALID_DESCRIPTOR                  0xE0001108L

#define USBIO_ERR_NOT_SUPPORTED_UNDER_CE              0xE0001109L


#endif // _USBIOERR_H_
/************************************* EOF **************************************/

