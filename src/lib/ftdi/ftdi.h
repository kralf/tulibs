/***************************************************************************
 *   Copyright (C) 2008 by Ralf Kaestner                                   *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef FTDI_H
#define FTDI_H

/** \file ftdi.h
  * \brief FTDI communication library
  * \author Ralf Kaestner
  * A communication library providing very basic support for FTDI's USB to
  * serial UART interfaces.
  */

#include <unistd.h>

/** \brief Predefined FTDI vendor identifier
  */
#define FTDI_VENDOR_ID                      0x0403

/** \name Error Codes
  * \brief Predefined FTDI error codes
  */
//@{
#define FTDI_ERROR_NONE                     0
#define FTDI_ERROR_CONTEXT_INIT             1
#define FTDI_ERROR_CONTEXT_RELEASE          2
#define FTDI_ERROR_INVALID_CONTEXT          3
#define FTDI_ERROR_OPEN                     4
#define FTDI_ERROR_CLOSE                    5
#define FTDI_ERROR_PURGE                    6
#define FTDI_ERROR_INVALID_INTERFACE        7
#define FTDI_ERROR_INVALID_BAUDRATE         8
#define FTDI_ERROR_INVALID_DATABITS         9
#define FTDI_ERROR_INVALID_STOPBITS         10
#define FTDI_ERROR_INVALID_PARITY           11
#define FTDI_ERROR_INVALID_FLOW_CTRL        12
#define FTDI_ERROR_INVALID_BREAK            13
#define FTDI_ERROR_INVALID_LATENCY          14
#define FTDI_ERROR_SETUP                    15
#define FTDI_ERROR_TIMEOUT                  16
#define FTDI_ERROR_READ                     17
#define FTDI_ERROR_WRITE                    18
//@}

/** \brief Predefined FTDI error descriptions
  */
extern const char* ftdi_errors[];

/** \brief Chip enumeratable type
  */
typedef enum {
  ftdi_chip_am,                 //!< FTDI AM chip series.
  ftdi_chip_bm,                 //!< FTDI BM chip series.
  ftdi_chip_2232c,              //!< FTDI 2232C chip series.
  ftdi_chip_r,                  //!< FTDI R chip series.
  ftdi_chip_2232h,              //!< FTDI 2232H chip series.
  ftdi_chip_4232h,              //!< FTDI 4232H chip series.
  ftdi_chip_232h,               //!< FTDI 232H chip series.
  ftdi_chip_unkown              //!< Unkown chip series.
} ftdi_chip_t;

/** \brief Predefined FTDI chip descriptions
  */
extern const char* ftdi_chips[];

/** \brief Interface enumeratable type
  */
typedef enum {
  ftdi_interface_any = 0,       //!< Any interface.
  ftdi_interface_a = 1,         //!< Interface A.
  ftdi_interface_b = 2,         //!< Interface B.
  ftdi_interface_c = 3,         //!< Interface C.
  ftdi_interface_d = 4          //!< Interface D.
} ftdi_interface_t;

/** \brief Parity enumeratable type
  */
typedef enum {
  ftdi_parity_none = 0,         //!< No parity.
  ftdi_parity_odd = 1,          //!< Odd parity.
  ftdi_parity_even = 2,         //!< Even parity.
  ftdi_parity_mark = 3,         //!< Mark parity.
  ftdi_parity_space = 4         //!< Space parity.
} ftdi_parity_t;

/** \brief Flow control enumeratable type
  */
typedef enum {
  ftdi_flow_ctrl_off = 0,       //!< Disable flow control.
  ftdi_flow_ctrl_xon_xoff = 1,  //!< XON/XOFF flow control.
  ftdi_flow_ctrl_rts_cts = 2,   //!< RTS/CTS (hardware) flow control.
  ftdi_flow_ctrl_dtr_dsr = 3    //!< DTR/DSR (hardware) flow control.
} ftdi_flow_ctrl_t;

/** \brief Break enumeratable type
  */
typedef enum {
  ftdi_break_off = 0,           //!< Break off.
  ftdi_break_on = 1             //!< Break on.
} ftdi_break_t;

/** \brief FTDI device structure
  */
typedef struct ftdi_device_t {
  void* libftdi_context;          //!< The libftdi context.
  void* libusb_device;            //!< The libusb device.

  int bus;                        //!< Number of bus device is connected to.
  int address;                    //!< Address on bus device is connected to.

  int product_id;                 //!< Product ID of device.
  ftdi_chip_t chip;               //!< Device chip type.
  ftdi_interface_t interface;     //!< Device interface number.
    
  int baudrate;                   //!< Device baudrate.
  int databits;                   //!< Number of databits.
  int stopbits;                   //!< Number of stopbits.
  ftdi_parity_t parity;           //!< Device parity.
  ftdi_flow_ctrl_t flow_ctrl;     //!< Device flow control.
  ftdi_break_t break_type;        //!< Device break enabled.

  unsigned char latency;          //!< Device latency in [ticks].

  size_t num_read;                //!< Number of bytes read from device.
  size_t num_written;             //!< Number of bytes written to device.
} ftdi_device_t, *ftdi_device_p;

/** \brief FTDI context structure
  */
typedef struct ftdi_context_t {
  size_t num_devices;             //!< Number of devices in the context.
  ftdi_device_t* devices;         //!< List of devices in the context.
  
  size_t num_references;          //!< Number of references to the context.
} ftdi_context_t, *ftdi_context_p;

/** \brief FTDI default context
  * \note The FTDI default context is a special context which will be
  *   shared amongst its users.
  */
extern ftdi_context_p ftdi_default_context;

/** \brief Initialize an FTDI context
  * \param[in] context The FTDI context to be initialized.
  * \return The resulting error code.
  */
int ftdi_context_init(
  ftdi_context_p context);

/** \brief Release an FTDI context
  * \param[in] context The initialized FTDI context to be released.
  * \return The resulting error code.
  */
int ftdi_context_release(
  ftdi_context_p context);

/** \brief Refresh device list of the FTDI context
  * \param[in] context The initialized FTDI context to be refreshed.
  * \return The resulting error code.
  */
int ftdi_context_refresh(
  ftdi_context_p context);

/** \brief Match FTDI devices by device name
  * \param[in] context The initialized FTDI context to be searched.
  * \param[in] name The device's udev name.
  * \return The matching device or null.
  */
ftdi_device_p ftdi_match_name(
  ftdi_context_p context,
  const char* name);

/** \brief Match FTDI devices by product ID
  * \param[in] context The initialized FTDI context to be searched.
  * \param[in] product_id The device's product ID to be matched.
  * \return The first matching device or null.
  */
ftdi_device_p ftdi_match_product(
  ftdi_context_p context,
  int product_id);

/** \brief Match FTDI devices by chip
  * \param[in] context The initialized FTDI context to be searched.
  * \param[in] chip The device's chip to be matched.
  * \return The first matching device or null.
  */
ftdi_device_p ftdi_match_chip(
  ftdi_context_p context,
  ftdi_chip_t chip);

/** \brief Open the FTDI device with the specified name
  * \param[in] dev The FTDI device to be opened.
  * \param[in] name The name of the device to be opened.
  * \param[in] interface The device interface to be opened.
  * \return The resulting error code.
  */
int ftdi_open(
  ftdi_device_p dev,
  const char* name,
  ftdi_interface_t interface);

/** \brief Close an open FTDI device
  * \param[in] dev The open FTDI device to be closed.
  * \return The resulting error code.
  */
int ftdi_close(
  ftdi_device_p dev);

/** \brief Setup an already opened FTDI device
  * \param[in] dev The open FTDI device to be set up.
  * \param[in] baudrate The device baudrate to be set.
  * \param[in] databits The device's number of databits to be set.
  * \param[in] stopbits The device's number of stopbits to be set.
  * \param[in] parity The device parity to be set.
  * \param[in] flow_ctrl The device flow control to be set.
  * \param[in] break_type The device break type to be set.
  * \param[in] latency The device latency timeout to be set in [ticks].
  * \return The resulting error code.
  */
int ftdi_setup(
  ftdi_device_p dev,
  int baudrate,
  int databits,
  int stopbits,
  ftdi_parity_t parity,
  ftdi_flow_ctrl_t flow_ctrl,
  ftdi_break_t break_type,
  unsigned char latency);

/** \brief Read data from open FTDI device
  * \param[in] dev The open FTDI device to read data from.
  * \param[in,out] data An array containing the data read from the device.
  * \param[in] num The number of data bytes to be read.
  * \return The number of bytes read from the FTDI device or the
  *   negative error code.
  */
int ftdi_read(
  ftdi_device_p dev,
  unsigned char* data,
  size_t num);

/** \brief Write data to open FTDI device
  * \param[in] dev The open FTDI device to write data to.
  * \param[in] data An array containing the data to be written to the device.
  * \param[in] num The number of data bytes to be written.
  * \return The number of bytes written to the FTDI device or the
  *   negative error code.
  */
int ftdi_write(
  ftdi_device_p dev,
  unsigned char* data,
  size_t num);

/** \brief Print an FTDI device
  * \param[in] stream The output stream that will be used for printing the
  *   FTDI device.
  * \param[in] dev The FTDI device that will be printed.
  */
void ftdi_print(
  FILE* stream,
  ftdi_device_p dev);

#endif
