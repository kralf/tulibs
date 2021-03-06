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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>

#include "config/param.h"

#include "error/error.h"

/** \defgroup config Configuration Module
  * \brief Library functions for managing configuration parameters
  * 
  * The configuration module provides library functions for aggregating
  * and accessing configuration parameters, for parsing their values from
  * command line arguments, and for generating command line help as well
  * as Linux manual pages to document their properties.
  */

/** \file config/config.h
  * \ingroup config
  * \brief Simple configuration implementation
  * \author Ralf Kaestner
  * 
  * A configuration represents an aggregate of configuration parameters
  * which can be accessed through their keys.
  */

/** \name Arguments
  * \brief Predefined configuration arguments
  */
//@{
#define CONFIG_ARG_HELP                         "--help"
//@}

/** \name Error Codes
  * \brief Predefined configuration error codes
  */
//@{
#define CONFIG_ERROR_NONE                       0
//!< Success
#define CONFIG_ERROR_PARAM_KEY                  1
//!< Invalid parameter key
#define CONFIG_ERROR_PARAM_VALUE_TYPE           2
//!< Parameter value type mismatch
#define CONFIG_ERROR_PARAM_VALUE_RANGE          3
//!< Parameter value out of range
//@}

/** \brief Predefined configuration error descriptions
  */
extern const char* config_errors[];

/** \brief Configuration structure
  */
typedef struct config_t {
  config_param_t* params; //!< The configuration parameters.
  size_t num_params;      //!< The number of configuration parameters.
  
  error_t error;          //!< The most recent configuration error.
} config_t;

/** \brief Default configuration structure
  *
  * The default configuration is immutable, but allows for static
  * initialization. 
  */
typedef struct config_default_t {
  const config_param_t*
    const params;         //!< The default configuration parameters.
  size_t num_params;      //!< The number of default configuration parameters.
} config_default_t;

/** \brief Initialize an empty configuration
  * \param[in] config The configuration to be initialized.
  */
void config_init(
  config_t* config);

/** \brief Initialize a configuration from defaults
  * \param[in] config The configuration to be initialized from defaults.
  * \param[in] defaults The default configuration used to initialize
  *   the configuration.
  */
void config_init_default(
  config_t* config,
  const config_default_t* defaults);

/** \brief Initialize a configuration from parameters
  * \param[in] config The configuration to be initialized.
  * \param[in] params The parameters used to initialize the configuration.
  * \param[in] num_params The number of parameters used to initialize the
  *   configuration.
  */
void config_init_params(
  config_t* config,
  const config_param_t* params,
  size_t num_params);

/** \brief Initialize a configuration by copying
  * \param[in] config The configuration to be initialized.
  * \param[in] src_config The source configuration used to initialize the
  *   configuration.
  */
void config_init_copy(
  config_t* config,
  const config_t* src_config);

/** \brief Destroy a configuration
  * \param[in] config The configuration to be destroyed.
  */
void config_destroy(
  config_t* config);

/** \brief Copy a configuration
  * \param[in] dst The destination configuration.
  * \param[in] src The source configuration.
  * 
  * When copying a source configuration to a destination configuration,
  * all paramters in the destination configuration will be destroyed.
  */
void config_copy(
  config_t* dst,
  const config_t* src);

/** \brief Copy a default configuration
  * \param[in] dst The destination configuration.
  * \param[in] src The default source configuration.
  * 
  * When copying a default source configuration to a destination configuration,
  * all paramters in the destination configuration will be destroyed.
  */
void config_copy_default(
  config_t* dst,
  const config_default_t* src);

/** \brief Copy configuration parameters
  * \param[in] dst The destination configuration.
  * \param[in] params The source configuration parameters.
  * \param[in] num_params The number of source configuration parameters.
  * 
  * When copying configuration parameters to a destination configuration,
  * all paramters in the destination configuration will be destroyed.
  */
void config_copy_params(
  config_t* dst,
  const config_param_t* params,
  size_t num_params);

/** \brief Clear a configuration
  * \param[in] config The configuration to be cleared.
  */
void config_clear(
  config_t* config);

/** \brief Merge configurations
  * \param[in] dst The destination configuration.
  * \param[in] src The source configuration.
  * 
  * When merging a source configuration into a destination configuration,
  * all non-conflicting parameters in the destination configuration will
  * be maintained. Any destination parameter with a key conflict will be
  * replaced by its corresponding source parameter.
  */
void config_merge(
  config_t* dst,
  const config_t* src);

/** \brief Print a configuration
  * \param[in] stream The output stream that will be used for printing the
  *   configuration.
  * \param[in] config The configuration that will be printed.
  */
void config_print(
  FILE* stream,
  const config_t* config);

/** \brief Set configuration parameter values from a source configuration
  * \param[in] dst The configuration to set the parameter values for.
  * \param[in] src The configuration containing the source parameter
  *    values to be set.
  * \return The resulting error code.
  */
int config_set(
  config_t* dst,
  const config_t* src);

/** \brief Set a configuration parameter
  * \note Calling this function may invalidate previously acquired parameter
  *   pointers.
  * \param[in] config The configuration to set the parameter for.
  * \param[in] param The parameter to be set.
  * \return The modified or inserted configuration parameter.
  * 
  * If a parameter with the same key already exists in the configuration,
  * it will be replaced. Otherwise, the parameter will be inserted into
  * the configuration. Therefore, the configuration's parameters will be
  * re-allocated to accommodate the inserted parameter.
  */
config_param_t* config_set_param(
  config_t* config,
  const config_param_t* param);

/** \brief Set a configuration parameter's value and range
  * \note Calling this function may invalidate previously acquired parameter
  *   pointers.
  * \param[in] config The configuration to set the parameter for.
  * \param[in] key The key of the parameter to be set.
  * \param[in] type The type of the parameter value to be set.
  * \param[in] value The string representation of the parameter's value.
  * \param[in] range An optional formal string expression representing
  *   the permissible range of the parameter value.
  * \param[in] description An optional description of the parameter.
  * \return The modified or inserted configuration parameter.
  * 
  * This function first initializes a new parameter by calling
  * config_param_init_value_range() with the provided arguments and
  * then attempts setting this parameter through config_set_param().
  */
config_param_t* config_set_param_value_range(
  config_t* config,
  const char* key,
  config_param_type_t type,
  const char* value,
  const char* range,
  const char* description);

/** \brief Retrieve a configuration parameter
  * \param[in] config The configuration to retrieve the parameter from.
  * \param[in] key The key of the parameter to be retrieved.
  * \return The configuration parameter with the specified key or null
  *   if no such parameter exists in the configuration.
  */
config_param_t* config_get_param(
  const config_t* config,
  const char* key);

/** \brief Set a configuration parameter's value
  * \param[in] config The configuration to set the value for.
  * \param[in] key The key of the string value to be set.
  * \param[in] value The string representation of the value to be set.
  * \return The resulting error code.
  * 
  * This function calls config_param_set_value() for the parameter with
  * the specified key.
  */
int config_set_value(
  config_t* config,
  const char* key,
  const char* value);

/** \brief Set a configuration parameter's string value
  * \param[in] config The configuration to set the string value for.
  * \param[in] key The key of the string value to be set.
  * \param[in] value The string value to be set.
  * \return The resulting error code.
  * 
  * This function calls config_param_set_string() for the parameter with
  * the specified key.
  */
int config_set_string(
  config_t* config,
  const char* key,
  const char* value);

/** \brief Retrieve a configuration parameter's string value
  * \param[in] config The configuration to retrieve the string value from.
  * \param[in] key The key of the string value to be retrieved.
  * \return The parameter's string value or null if no such parameter
  *   exists in the configuration.
  * 
  * This function calls config_param_get_string() for the parameter with
  * the specified key.
  */
const char* config_get_string(
  const config_t* config,
  const char* key);

/** \brief Set a configuration parameter's integer value
  * \param[in] config The configuration to set the integer value for.
  * \param[in] key The key of the integer value to be set.
  * \param[in] value The integer value to be set.
  * \return The resulting error code.
  * 
  * This function calls config_param_set_int() for the parameter with the
  * specified key.
  */
int config_set_int(
  config_t* config,
  const char* key,
  int value);

/** \brief Retrieve a configuration parameter's integer value
  * \param[in] config The configuration to retrieve the integer value from.
  * \param[in] key The key of the integer value to be retrieved.
  * \return The parameter's integer value or zero if no such parameter
  *   exists in the configuration.
  * 
  * This function calls config_param_get_int() for the parameter with the
  * specified key.
  */
int config_get_int(
  const config_t* config,
  const char* key);

/** \brief Set a configuration parameter's floating point value
  * \param[in] config The configuration to set the floating point value for.
  * \param[in] key The key of the floating point value to be set.
  * \param[in] value The floating point value to be set.
  * \return The resulting error code.
  * 
  * This function calls config_param_set_float() for the parameter with the
  * specified key.
  */
int config_set_float(
  config_t* config,
  const char* key,
  double value);

/** \brief Retrieve a configuration parameter's floating point value
  * \param[in] config The configuration to retrieve the floating point
  *   value from.
  * \param[in] key The key of the floating point value to be retrieved.
  * \return The parameter's floating point value or NaN if no such parameter
  *   exists in the configuration.
  * 
  * This function calls config_param_get_float() for the parameter with the
  * specified key.
  */
double config_get_float(
  const config_t* config,
  const char* key);

/** \brief Set a configuration parameter's enumerable value
  * \param[in] config The configuration to set the enumerable value for.
  * \param[in] key The key of the enumerable value to be set.
  * \param[in] value The enumerable value to be set.
  * \return The resulting error code.
  * 
  * This function calls config_param_set_enum() for the parameter with the
  * specified key.
  */
int config_set_enum(
  config_t* config,
  const char* key,
  int value);

/** \brief Retrieve a configuration parameter's enumerable value
  * \param[in] config The configuration to retrieve the enumerable
  *   value from.
  * \param[in] key The key of the enumerable value to be retrieved.
  * \return The parameter's enumerable value or -1 if no such parameter
  *   exists in the configuration.
  * 
  * This function calls config_param_get_enum() for the parameter with the
  * specified key.
  */
int config_get_enum(
  const config_t* config,
  const char* key);

/** \brief Set a configuration parameter's boolean value
  * \param[in] config The configuration to set the boolean value for.
  * \param[in] key The key of the boolean value to be set.
  * \param[in] value The boolean value to be set.
  * \return The resulting error code.
  * 
  * This function calls config_param_set_bool() for the parameter with the
  * specified key.
  */
int config_set_bool(
  config_t* config,
  const char* key,
  config_param_bool_t value);

/** \brief Retrieve a configuration parameter's boolean value
  * \param[in] config The configuration to retrieve the boolean
  *   value from.
  * \param[in] key The key of the boolean value to be retrieved.
  * \return The parameter's boolean value or false if no such parameter
  *   exists in the configuration.
  * 
  * This function calls config_param_get_bool() for the parameter with the
  * specified key.
  */
config_param_bool_t config_get_bool(
  const config_t* config,
  const char* key);

#endif
