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

#include <string.h>
#include <math.h>

#include "config.h"

#include "string/string.h"

const char* config_errors[] = {
  "Success",
  "Parameter key error",
  "Parameter value error",
};

void config_init(config_t* config) {
  config->params = 0;
  config->num_params = 0;
}

void config_init_copy(config_t* config, const config_t* src_config) {
  config_init(config);
  config_copy(config, src_config);
}

void config_destroy(config_t* config) {
  config_clear(config);
}

void config_copy(config_t* dst, const config_t* src) {
  if (src->num_params) {
    size_t i;
    
    for (i = src->num_params; i < dst->num_params; ++i)
      config_param_destroy(&dst->params[i]);
    dst->params = realloc(dst->params,
      src->num_params*sizeof(config_param_t));
    for (i = 0; i < src->num_params; ++i) {
      if (i < dst->num_params)
        config_param_copy(&dst->params[i], &src->params[i]);
      else
        config_param_init_copy(&dst->params[i], &src->params[i]);
    }
    
    dst->num_params = src->num_params;
  }
  else if (dst->num_params)
    config_clear(dst);
}

void config_clear(config_t* config) {
  if (config->num_params) {
    size_t i;

    for (i = 0; i < config->num_params; ++i)
      config_param_destroy(&config->params[i]);
    
    free(config->params);
    config->params = 0;
    config->num_params = 0;
  }
}

void config_merge(config_t* dst, const config_t* src) {
  size_t i;
  
  for (i = 0; i < src->num_params; ++i)
    config_set_param(dst, &src->params[i]);
}

void config_print(FILE* stream, const config_t* config) {
  size_t i;

  for (i = 0; i < config->num_params; ++i)
    config_param_print(stream, &config->params[i]);
}

int config_set(config_t* dst, const config_t* src) {
  size_t i;
  int result;

  for (i = 0; i < src->num_params; ++i) {
    const config_param_t* src_param = &src->params[i];
    if ((result = config_set_value(dst, src_param->key, src_param->value)))
      return result;
  }
  
  return CONFIG_ERROR_NONE;
}

config_param_t* config_set_param(config_t* config, const config_param_t*
    param) {
  config_param_t* config_param = config_get_param(config, param->key);

  if (!config_param) {
    config->params = realloc(config->params, (config->num_params+1)*
      sizeof(config_param_t));

    config_param = &config->params[config->num_params];
    config_param_init_copy(config_param, param);
  
    ++config->num_params;
  }
  else
    config_param_copy(config_param, param);
  
  return config_param;
}

config_param_t* config_set_param_value_range(config_t* config, const char*
    key, config_param_type_t type, const char* value, const char* range,
    const char* description) {
  config_param_t param;  
  config_param_init_value_range(&param, key, type, value, range,
    description);

  return config_set_param(config, &param);
}

config_param_t* config_get_param(const config_t* config, const char* key) {
  size_t i;

  for (i = 0; i < config->num_params; ++i)
    if (string_equal(config->params[i].key, key))
      return &config->params[i];

  return 0;
}

int config_set_value(config_t* config, const char* key, const char* value) {
  config_param_t* param = config_get_param(config, key);

  if (param) {
    if (config_param_set_value(param, value))
      return CONFIG_ERROR_PARAM_VALUE;
  }
  else
    return CONFIG_ERROR_PARAM_KEY;
  
  return CONFIG_ERROR_NONE;
}

int config_set_string(config_t* config, const char* key, const char* value) {
  config_param_t* param = config_get_param(config, key);

  if (param) {
    if (config_param_set_string(param, value))
      return CONFIG_ERROR_PARAM_VALUE;
  }
  else
    return CONFIG_ERROR_PARAM_KEY;
  
  return CONFIG_ERROR_NONE;
}

const char* config_get_string(const config_t* config, const char* key) {
  const config_param_t* param = config_get_param(config, key);

  if (param)
    return config_param_get_string(param);
  else
    return 0;
}

int config_set_int(config_t* config, const char* key, int value) {
  config_param_t* param = config_get_param(config, key);

  if (param) {
    if (config_param_set_int(param, value))
      return CONFIG_ERROR_PARAM_VALUE;
  }
  else
    return CONFIG_ERROR_PARAM_KEY;
  
  return CONFIG_ERROR_NONE;
}

int config_get_int(const config_t* config, const char* key) {
  const config_param_t* param = config_get_param(config, key);

  if (param)
    return config_param_get_int(param);
  else
    return 0;
}

int config_set_float(config_t* config, const char* key, double value) {
  config_param_t* param = config_get_param(config, key);

  if (param) {
    if (config_param_set_float(param, value))
      return CONFIG_ERROR_PARAM_VALUE;
  }
  else
    return CONFIG_ERROR_PARAM_KEY;
  
  return CONFIG_ERROR_NONE;
}

double config_get_float(const config_t* config, const char* key) {
  const config_param_t* param = config_get_param(config, key);

  if (param)
    return config_param_get_float(param);
  else
    return NAN;
}

int config_set_enum(config_t* config, const char* key, int value) {
  config_param_t* param = config_get_param(config, key);

  if (param) {
    if (config_param_set_enum(param, value))
      return CONFIG_ERROR_PARAM_VALUE;
  }
  else
    return CONFIG_ERROR_PARAM_KEY;
  
  return CONFIG_ERROR_NONE;
}

int config_get_enum(const config_t* config, const char* key) {
  const config_param_t* param = config_get_param(config, key);

  if (param)
    return config_param_get_enum(param);
  else
    return -1;
}

int config_set_bool(config_t* config, const char* key, config_param_bool_t
    value) {
  config_param_t* param = config_get_param(config, key);

  if (param) {
    if (config_param_set_bool(param, value))
      return CONFIG_ERROR_PARAM_VALUE;
  }
  else
    return CONFIG_ERROR_PARAM_KEY;
  
  return CONFIG_ERROR_NONE;
}

config_param_bool_t config_get_bool(const config_t* config, const char* key) {
  const config_param_t* param = config_get_param(config, key);

  if (param)
    return config_param_get_bool(param);
  else
    return config_param_false;
}
