#ifndef FINGERDEFINITION_H
#define FINGERDEFINITION_H

#define PORT_DESC "CP2102 USB to UART Bridge Controller"  //* Change this to the actual description of your

//* Header definitions for the fingerprint sensor communication protocol
#define FIRST_HEADER  0xEF
#define SECOND_HEADER 0x01

//* Default address
#define DEFAULT_ADDRESS 0xFF

//* Command definitions for the fingerprint sensor
#define COMMAND_PACKAGE     0x01
#define DATA_PACKEGE        0x02
#define ACK_PACKAGE         0x07
#define END_OF_DATA_PACKAGE 0x08

#define COMMAND_EXECUTION_COMPLETE        0x00
#define ERROR_WHEN_RECEIVE_DATA_PACKAGE   0x01
#define NO_FINGER_ON_THE_SENSOR           0x02
#define FAIL_TO_ENROLL_THE_FINGER         0x03
#define FAIL_DUE_TO_OVER_DISORDERLY_IMAGE 0x06
#define FAIL_DUE_TO_WEAK_OR_SMALL_IMAGE   0x07
#define FINGER_DOES_NOT_MATCH             0x08
#define FAIL_TO_FIND_MATCHING_FINGER      0x09
#define FAIL_TO_COMBINE_CHARACTER_FILES   0x0A
#define PAGEID_OUT_OF_RANGE               0x0B
#define READ_TEMPLATE_ERROR_OR_INVALID    0x0C
#define ERROR_WHEN_UPLOADING_TEMPLATE     0x0D
#define CANNOT_RECEIVE_FOLLOWING_PACKAGES 0x0E
#define ERROR_WHEN_UPLOADING_IMAGE        0x0F
#define FAIL_TO_DELETE_TEMPLATE           0x10
#define FAIL_TO_CLEAR_FINGER_LIBRARY      0x11
#define FAIL_DUE_TO_WEAK_PRIMARY_IMAGE    0x15
#define ERROR_WHEN_WRITING_FLASH          0x18
#define INVALID_REGISTER_NUMBER           0x1A

#endif  // FINGERDEFINITION_H