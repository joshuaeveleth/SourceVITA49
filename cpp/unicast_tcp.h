/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK core.
 *
 * REDHAWK core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK core is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef UNICAST_TCP_H_
#define UNICAST_TCP_H_

#include <arpa/inet.h>
#include <stdexcept>
#include "unicast.h"

#ifdef __cplusplus
extern "C" {
#endif

unicast_t unicast_tcp_client (const char* iface, const char* group, int port);
ssize_t unicast_tcp_receive (unicast_t client, void* buffer, size_t bytes, unsigned int to_in_msecs= 0);
unicast_t unicast_tcp_server (const char* iface, const char* group, int port);
ssize_t unicast_tcp_transmit (unicast_t server, const void* buffer, size_t bytes);
ssize_t unicast_tcp_transmit_to_client (unicast_t client, const void* buffer, size_t bytes);
int unicast_tcp_wait_for_client (unicast_t server);
int unicast_tcp_poll_in (unicast_t client, int timeout);
void unicast_tcp_close(unicast_t socket);

#ifdef __cplusplus
}
#endif



#endif /* UNICAST_TCP_H_ */
