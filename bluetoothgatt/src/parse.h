/*
 * $QNXLicenseC:
 * Copyright 2013, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.   Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

/**
 * @file parse.h
 *
 * @brief Functions for parsing low energy data into human-readable form.
 *
 */

#ifndef PARSE_H_
#define PARSE_H_

__BEGIN_DECLS

const char *get_service_name(const char *uuid);
const char *get_characteristic_name(const char *uuid);
const char *get_descriptor_name(const char *uuid);
int parse_characteristic_uuid_buffer(const char *uuid, const uint8_t *in, int in_len, char *out, int out_len);

__END_DECLS

#endif

__SRCVERSION( "$URL$ $Rev$" )

