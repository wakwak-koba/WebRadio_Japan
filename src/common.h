/*
 * https://twitter.com/wakwak_koba/
 */

#ifndef _WAKWAK_KOBA_WEBRADIO_COMMON_H_
#define _WAKWAK_KOBA_WEBRADIO_COMMON_H_

#include <Arduino.h>
#include "uzlib/uzlib.h"


static void getInner(const String & source, const String & tagF, const String & tagT, std::function<void(const String &)> action, bool removeTagF = false) {
  int idxF = 0;
  for(;;) {
    idxF = source.indexOf(tagF, idxF);
    int idxT = source.indexOf(tagT, idxF + tagF.length());
    if(idxF < 0 || idxT < 0) break;
    action(source.substring(idxF + (removeTagF ? tagF.length() : 0), idxT));
    idxF = idxT + tagT.length();
  }
}

static void getInner(Stream * source, const String & tagF, const String & tagT, std::function<void(const String &)> action, bool removeTagF = false) {
  for(;;) {
    if(!source->find(tagF.c_str()))
      break;
    
    String value;
    if(!removeTagF)
      value = tagF;
    
    for(;;) {
      auto c = source->read();
      if(c < 0) {
        action(value);
        return;
      }
      value.concat((char)c);
      if(value.endsWith(tagT)) {
        action(value.substring(0, value.length() - tagT.length()));
        break;
      }
    }
  }
}

static void getInner(const String & source, const String & tag, std::function<void(const String &)> action) {
  getInner(source, "<" + tag + ">", "</" + tag + ">", action, true);
}

static void getInner(Stream * source, const String & tag, std::function<void(const String &)> action) {
  getInner(source, "<" + tag + ">", "</" + tag + ">", action, true);
}

static uint8_t asc2byte(const char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr))
    rVal = chr - '0';
  else if (chr >= 'A' && chr <= 'F')
    rVal = chr + 10 - 'A';
  else if (chr >= 'a' && chr <= 'f')
    rVal = chr + 10 - 'a';
  return rVal;
}

static void unHex(const char * inP, uint8_t * outP, size_t len) {
  for (; len > 1; len -= 2) {
    uint8_t val = asc2byte(*inP++) << 4;
    *outP++ = val | asc2byte(*inP++);
  }
}

static String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}

static String uncompress(uint8_t * source, const size_t len) {
  size_t dlen;
  dlen =            source[len - 1];
  dlen = 256*dlen + source[len - 2];
  dlen = 256*dlen + source[len - 3];
  dlen = 256*dlen + source[len - 4];
  auto outlen = dlen;
  dlen++;
  auto dest = (uint8_t *)malloc(dlen);
  if(dest == nullptr)
  return String("");
  
  struct uzlib_uncomp d;
  uzlib_uncompress_init(&d, NULL, 0);
  
  d.source = source;
  d.source_limit = source + len - 4;
  d.source_read_cb = nullptr;

  auto res = uzlib_gzip_parse_header(&d);
  if (res != TINF_OK)
      return String("");

  d.dest_start = d.dest = dest;

  while (dlen) {
      auto chunk_len = 1;
      d.dest_limit = d.dest + chunk_len;
      res = uzlib_uncompress_chksum(&d);
      dlen -= chunk_len;
      if (res != TINF_OK) {
          break;
      }
  }  
  
  if (res != TINF_DONE)
    return String("");
  
  dest[outlen] = 0;
  auto result = String((char *)dest);
  free(dest);
  return result;
}

static String htmlDecode(String source) {
  source.replace("&amp;", "&");
  source.replace("&lt;", "<");
  source.replace("&gt;", ">");
  source.replace("&quot;", "\"");
  source.replace("&apos;", "'");
  return source;
}

#endif