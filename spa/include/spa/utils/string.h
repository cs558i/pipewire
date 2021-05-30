/* Simple Plugin API
 *
 * Copyright © 2021 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef SPA_UTILS_STRING_H
#define SPA_UTILS_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>

#include <spa/utils/defs.h>

/**
 * \addtogroup spa_utils
 * \{
 */

/**
 * \return true if the two strings are equal, false otherwise
 *
 * If both \a a and \a b are NULL, the two are considered equal.
 *
 * \ingroup spa
 */
static inline bool spa_streq(const char *s1, const char *s2)
{
	return SPA_LIKELY(s1 && s2) ? strcmp(s1, s2) == 0 : s1 == s2;
}

/**
 * \return true if the two strings are equal, false otherwise
 *
 * If both \a a and \a b are NULL, the two are considered equal.
 */
static inline bool spa_strneq(const char *s1, const char *s2, size_t len)
{
	return SPA_LIKELY(s1 && s2) ? strncmp(s1, s2, len) == 0 : s1 == s2;
}

/**
 * Convert \a str to an int32_t with the given \a base and store the
 * result in \a val.
 *
 * On failure, the value of \a val is unmodified.
 *
 * \return true on success, false otherwise
 */
static inline bool spa_atoi32(const char *str, int32_t *val, int base)
{
	char *endptr;
	long v;

	if (!str || *str =='\0')
		return false;

	errno = 0;
	v = strtol(str, &endptr, base);
	if (errno != 0 || *endptr != '\0')
		return false;

	if (v != (int32_t)v)
		return false;

	*val = v;
	return true;
}

/**
 * Convert \a str to an uint32_t with the given \a base and store the
 * result in \a val.
 *
 * On failure, the value of \a val is unmodified.
 *
 * \return true on success, false otherwise
 */
static inline bool spa_atou32(const char *str, uint32_t *val, int base)
{
	char *endptr;
	unsigned long long v;

	if (!str || *str =='\0')
		return false;

	errno = 0;
	v = strtoull(str, &endptr, base);
	if (errno != 0 || *endptr != '\0')
		return false;

	if (v != (uint32_t)v)
		return false;

	*val = v;
	return true;
}

/**
 * Convert \a str to an int64_t with the given \a base and store the
 * result in \a val.
 *
 * On failure, the value of \a val is unmodified.
 *
 * \return true on success, false otherwise
 */
static inline bool spa_atoi64(const char *str, int64_t *val, int base)
{
	char *endptr;
	long long v;

	if (!str || *str =='\0')
		return false;

	errno = 0;
	v = strtoll(str, &endptr, base);
	if (errno != 0 || *endptr != '\0')
		return false;

	*val = v;
	return true;
}

/**
 * Convert \a str to an uint64_t with the given \a base and store the
 * result in \a val.
 *
 * On failure, the value of \a val is unmodified.
 *
 * \return true on success, false otherwise
 */
static inline bool spa_atou64(const char *str, uint64_t *val, int base)
{
	char *endptr;
	unsigned long long v;

	if (!str || *str =='\0')
		return false;

	errno = 0;
	v = strtoull(str, &endptr, base);
	if (errno != 0 || *endptr != '\0')
		return false;

	*val = v;
	return true;
}

/**
 * Convert \a str to a boolean. Allowed boolean values are "true" and a
 * literal "1", anything else is false.
 *
 * \return true on success, false otherwise
 */
static inline bool spa_atob(const char *str)
{
	return spa_streq(str, "true") || spa_streq(str, "1");
}

/**
 * "Safe" version of vsnprintf. Exactly the same as vsnprintf but the
 * returned value is clipped to `size - 1` and a negative or zero size
 * will abort() the program.
 *
 * \return The number of bytes printed, capped to `size-1`, or a negative
 * number on error.
 */
SPA_PRINTF_FUNC(3, 0)
static inline int spa_vscnprintf(char *buffer, size_t size, const char *format, va_list args)
{
	int r;

	spa_assert((ssize_t)size > 0);

	r = vsnprintf(buffer, size, format, args);
	if (SPA_UNLIKELY(r < 0))
		buffer[0] = '\0';
	if (SPA_LIKELY(r < (ssize_t)size))
		return r;
	return size - 1;
}

/**
 * "Safe" version of snprintf. Exactly the same as snprintf but the
 * returned value is clipped to `size - 1` and a negative or zero size
 * will abort() the program.
 *
 * \return The number of bytes printed, capped to `size-1`, or a negative
 * number on error.
 */
SPA_PRINTF_FUNC(3, 4)
static inline int spa_scnprintf(char *buffer, size_t size, const char *format, ...)
{
	int r;
	va_list args;

	va_start(args, format);
	r = spa_vscnprintf(buffer, size, format, args);
	va_end(args);

	return r;
}

/**
 * Convert \a str to a float and store the result in \a val.
 *
 * On failure, the value of \a val is unmodified.
 *
 * \return true on success, false otherwise
 */
static inline bool spa_atof(const char *str, float *val)
{
	char *endptr;
	float v;

	if (!str || *str =='\0')
		return false;

	errno = 0;
	v = strtof(str, &endptr);
	if (errno != 0 || *endptr != '\0')
		return false;

	*val = v;
	return true;
}

/**
 * Convert \a str to a double and store the result in \a val.
 *
 * On failure, the value of \a val is unmodified.
 *
 * \return true on success, false otherwise
 */
static inline bool spa_atod(const char *str, double *val)
{
	char *endptr;
	double v;

	if (!str || *str =='\0')
		return false;

	errno = 0;
	v = strtod(str, &endptr);
	if (errno != 0 || *endptr != '\0')
		return false;

	*val = v;
	return true;
}

/**
 * \}
 */

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* SPA_UTILS_STRING_H */
