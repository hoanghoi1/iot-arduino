/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CoAp_conf.h
 * Author: Hoang Hoi
 *
 * Created on Ngày 05 tháng 3 năm 2017, 02:04
 */

/* +-----+---+---+---+---+----------------+--------+--------+----------+
   | No. | C | U | N | R | Name           | Format | Length | Default  |
   +-----+---+---+---+---+----------------+--------+--------+----------+
   |   1 | x |   |   | x | If-Match       | opaque | 0-8    | (none)   |
   |   3 | x | x | - |   | Uri-Host       | string | 1-255  | (see     |
   |     |   |   |   |   |                |        |        | below)   |
   |   4 |   |   |   | x | ETag           | opaque | 1-8    | (none)   |
   |   5 | x |   |   |   | If-None-Match  | empty  | 0      | (none)   |
   |   7 | x | x | - |   | Uri-Port       | uint   | 0-2    | (see     |
   |     |   |   |   |   |                |        |        | below)   |
   |   8 |   |   |   | x | Location-Path  | string | 0-255  | (none)   |
   |  11 | x | x | - | x | Uri-Path       | string | 0-255  | (none)   |
   |  12 |   |   |   |   | Content-Format | uint   | 0-2    | (none)   |
   |  14 |   | x | - |   | Max-Age        | uint   | 0-4    | 60       |
   |  15 | x | x | - | x | Uri-Query      | string | 0-255  | (none)   |
   |  16 |   |   |   | x | Accept         | uint   | 0-2    | (none)   |
   |  20 |   |   |   | x | Location-Query | string | 0-255  | (none)   |
   |  35 | x | x | - |   | Proxy-Uri      | string | 1-1034 | (none)   |
   |  39 | x | x | - |   | Proxy-Scheme   | string | 1-255  | (none)   |
   +-----+---+---+---+---+----------------+--------+--------+----------+
 */

#ifndef COAP_CONF_H
#define COAP_CONF_H

#ifdef __cplusplus
extern "C" {
#endif
#define COAP_PORT 5683
#define COAP_VERSION 1
    
#define COAP_CONFIRMABLE 0b00
#define COAP_NON_CONFIRMABLE 0b01
#define COAP_ACKNOWLEDGEMENT 0b10

#define COAP_GET 1
#define COAP_POST 2
#define COAP_PUT 3
#define COAP_DELETE 4

// response code 2.xx
#define COAP_CREATED 65
#define COAP_DELETED 66
#define COAP_VALID 67
#define COAP_CHANGED 68
#define COAP_CONTENT 69

// response code 4.xx
#define COAP_BAD_REQUEST 128
#define COAP_UNAUTHORIZED 129
#define COAP_BAD_OPTION 130
#define COAP_FORBIDDEN 131
#define COAP_NOT_FOUND 132
#define COAP_METHOD_NOT_ALLOWED 133
#define COAP_NOT_ACCEPTABLE 134

// response code 5.xx
#define COAP_INTERNAL_SERVER_ERROR 160
#define COAP_NOT_IMPLEMENTED 161
#define COAP_BAD_GATEWAY 162
#define COAP_SERVICE_UNAVAILABLE 163
#define COAP_GATEWAY_TIMEOUT 164

#define COAP_URI_PATH_FIRST 0xb0
#define COAP_URI_QUERY_FIRST 0x40
#define COAP_URI_QUERY_FIRST_NO_PATH 0xd0
#define COAP_URI_FOLLOW 0x00
#define COAP_URI_LENGTH 13
#define COAP_URI_LONG 0x0d
#define COAP_PRE_PAYLOAD 0xff


#ifdef __cplusplus
}
#endif

#endif /* COAP_CONF_H */

