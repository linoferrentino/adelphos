/*
 *
 * Adelphos, log utilities
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 */

#include <string.h>

#include "alog.h"


static void _log_put_module_name(const char *_buf, char *_imp_MODULE_)
{
  int len = strlen(_buf);
  int start_pos = len < _MODULE_LEN_FIX ? 0 : len - _MODULE_LEN_FIX;
  int start_pos_src = len < _MODULE_LEN_FIX ? _MODULE_LEN_FIX - len : 0;
  int i;
  for (i = 0 ; i < start_pos_src ; ++i){
    _imp_MODULE_[i] = '-';
  }
  int j = start_pos;
  for (i = start_pos_src ; i < _MODULE_LEN_FIX ; ++i){
    _imp_MODULE_[i] = _buf[j++];
  }
  _imp_MODULE_[_MODULE_LEN_FIX] = '\0';
}

const char *ad_get_module(const char* _buf, uint8_t *_imp_module_initialized_, char *_imp_MODULE_)
{
  if (__sync_bool_compare_and_swap(_imp_module_initialized_, 0, 1)){
    _log_put_module_name(_buf, _imp_MODULE_);
  }
  return _imp_MODULE_;
}

