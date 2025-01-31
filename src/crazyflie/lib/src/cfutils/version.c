/**
 *    ||          ____  _ __                           
 * +------+      / __ )(_) /_______________ _____  ___ 
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2011-2012 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Adapted from original file, using cmake to manually set the params with @ONLY - WEIJUN
 * 
 * version.c file: Auto generated via cmake, make edits in version.vtpl
 */
#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "param.h"

const char * V_SLOCAL_REVISION="26";
const char * V_SREVISION="43b6c25db2ee";
const char * V_STAG="2018.10-rc1-26";
const char * V_BRANCH="master";
// const char * V_PROFILE=P_NAME;
const bool V_MODIFIED=false;

/* Version recoverable from the ground */
const uint32_t V_REVISION_0=0x43b6c25d;
const uint16_t V_REVISION_1=0xb2ee;

PARAM_GROUP_START(firmware)
PARAM_ADD(PARAM_UINT32 | PARAM_RONLY, revision0, &V_REVISION_0)
PARAM_ADD(PARAM_UINT16 | PARAM_RONLY, revision1, &V_REVISION_1)
PARAM_ADD(PARAM_UINT8 | PARAM_RONLY, modified, &V_MODIFIED)
PARAM_GROUP_STOP(firmware)

