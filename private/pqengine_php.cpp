/****************************************************************************
**
** Copyright (C) 2015 WxMaper (http://wxmaper.ru)
**
** This file is part of the PQEngine.
**
** BEGIN LICENSE: MPL 2.0
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at http://mozilla.org/MPL/2.0/.
**
** END LICENSE
**
****************************************************************************/

#include "pqengine_private.h"
#include "phpqt5.h"

#include <QTextCodec>

int PQEnginePrivate::php_pqengine_startup(sapi_module_struct *sapi_module)
{
    Q_UNUSED(sapi_module)

    zend_module_entry entry[1];
    entry[0] = *PHPQt5::phpqt5_module_entry();

    if (php_module_startup(sapi_module, entry, 1) == FAILURE) {
        return FAILURE;
    }

    return SUCCESS;
}

int PQEnginePrivate::php_pqengine_deactivate()
{
    return SUCCESS;
}

size_t PQEnginePrivate::php_pqengine_ub_write(const char *str, size_t str_length)
{
    pq_ub_write(PHPQt5::toUTF8(str));
    return str_length;
}

void PQEnginePrivate::php_pqengine_error(int type, const char *format, ...)
{
#ifdef PQDEBUG
    PQDBG("PQEnginePrivate::php_pqengine_error()");
#endif

    Q_UNUSED(type)
    Q_UNUSED(format)
}

void PQEnginePrivate::php_pqengine_flush(void *server_context)
{
    Q_UNUSED(server_context)
}

void PQEnginePrivate::php_pqengine_send_header(sapi_header_struct *sapi_header, void *server_context)
{
    Q_UNUSED(sapi_header)
    Q_UNUSED(server_context)
}

void PQEnginePrivate::php_pqengine_register_variables(zval *track_vars_array)
{
#ifdef PQDEBUG
    PQDBG("PQEnginePrivate::php_pqengine_register_variables()");
#endif

    php_import_environment_variables(track_vars_array);
    php_register_variable((char*)"PHP_SELF", (char*)"", track_vars_array);
    php_register_variable((char*)"SCRIPT_NAME", (char*)"", track_vars_array);
    php_register_variable((char*)"SCRIPT_FILENAME", (char*)"", track_vars_array);
    php_register_variable((char*)"PATH_TRANSLATED", (char*)"", track_vars_array);
    php_register_variable((char*)"DOCUMENT_ROOT", (char*)"", track_vars_array);

    /*
    char buf[128];
    char **env, *p, *t = buf;
    size_t alloc_size = sizeof(buf);
    unsigned long nlen; /* ptrdiff_t is not portable *

    for (env = environ; env != NULL && *env != NULL; env++) {
        p = strchr(*env, '=');
        if (!p) {				/* malformed entry? *
            continue;
        }
        nlen = p - *env;
        if (nlen >= alloc_size) {
            alloc_size = nlen + 64;
            t = (char*) (t == buf ? emalloc(alloc_size): erealloc(t, alloc_size));
        }
        memcpy(t, *env, nlen);
        t[nlen] = '\0';
        php_register_variable(t, p + 1, track_vars_array);
    }
    if (t != buf && t != NULL) {
        efree(t);
    }
    */
}

#if (PHP_VERSION_ID < 70100)
void PQEnginePrivate::php_pqengine_log_message(char *message)
#else
void PQEnginePrivate::php_pqengine_log_message(char *message, int syslog_type_int)
#endif
{
#ifdef PQDEBUG
    PQDBG2("PQEnginePrivate::php_pqengine_log_message()", message);
#endif

#if (PHP_VERSION_ID >= 70100)
    Q_UNUSED(syslog_type_int)
#endif

    pq_ub_write(PHPQt5::toUTF8(message));
}
