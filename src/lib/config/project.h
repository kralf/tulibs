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

#ifndef CONFIG_PROJECT_H
#define CONFIG_PROJECT_H

#include <stdlib.h>

#include "config/config.h"

/** \file config/project.h
  * \ingroup config
  * \brief Project configuration support
  * \author Ralf Kaestner
  * 
  * The project configuration contains all vital information about a
  * software project. Such information may be provided programmatically
  * or by the build system.
  */

/** \brief Predefined project parser option group
  */
#define CONFIG_PROJECT_PARSER_OPTION_GROUP      "project"

/** \name Parameters
  * \brief Predefined project parameters
  */
//@{
#define CONFIG_PROJECT_PARAMETER_NAME           "name"
#define CONFIG_PROJECT_PARAMETER_VERSION        "version"
#define CONFIG_PROJECT_PARAMETER_AUTHORS        "authors"
#define CONFIG_PROJECT_PARAMETER_CONTACT        "contact"
#define CONFIG_PROJECT_PARAMETER_HOME           "home"
#define CONFIG_PROJECT_PARAMETER_LICENSE        "license"
//@}

/** \brief Predefined project default options
  */
extern const config_default_t config_project_default_options;

/** \brief Project configuration structure
  */
typedef struct config_project_t {
  char* name;                //!< The name of the project.
  char* version;             //!< The version of the project.
  
  char* authors;             //!< The project authors.
  char* contact;             //!< The project's maintainer contact.
  char* home;                //!< The project's home page.
  char* license;             //!< The project's license.
} config_project_t;

/** \brief Initialize project configuration
  * \param[in] project The project configuration to be initialized.
  * \param[in] name The name of the project to initialize the
  *   configuration for.
  * \param[in] version The version of the project to initialize the
  *   configuration for.
  * \param[in] authors The authors of the project to initialize the
  *   configuration for.
  * \param[in] contact The maintainer contact of the project to initialize
  *   the configuration for.
  * \param[in] home The home page of the project to initialize the
  *   configuration for.
  * \param[in] license The license of the project to initialize the
  *   configuration for.
  */
void config_project_init(
  config_project_t* project,
  const char* name,
  const char* version,
  const char* authors,
  const char* contact,
  const char* home,
  const char* license);

/** \brief Initialize project configuration from configuration parameters
  * \param[in] project The project configuration to be initialized.
  * \param[in] config The project configuration parameters used to
  *   initialize the project configuration.
  */
void config_project_init_config(
  config_project_t* project,
  const config_t* config);

/** \brief Initialize project configuration by copying
  * \param[in] project The project configuration to be initialized.
  * \param[in] src_project The source project configuration used to
  *   initialize the project configuration.
  */
void config_project_init_copy(
  config_project_t* project,
  const config_project_t* src_project);

/** \brief Destroy project configuration
  * \param[in] project The project configuration to be destroyed.
  */
void config_project_destroy(
  config_project_t* project);

#endif
