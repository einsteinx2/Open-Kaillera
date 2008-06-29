/******************************************************************************
          .d8888b.   .d8888b.  
         d88P  Y88b d88P  Y88b 
         888    888        888 
88888b.  888    888      .d88P 
888 "88b 888    888  .od888P"  
888  888 888    888 d88P"      
888  888 Y88b  d88P 888"       
888  888  "Y8888P"  888888888              Open Kaillera Arcade Netplay Library
*******************************************************************************
Copyright (c) Open Kaillera Team 2003-2008

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice must be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#pragma once

#include "SocketAddress.h"
#include "StaticArray.h"

#define ioctl ioctlsocket
#define socklen_t int

#define BIG_RECV_BUFFER_SIZE (32 << 10)

namespace n02 {

	/* bsd sockets class */

    class BsdSocket
    {

    protected:
		/* sockets descriptor
		value is SOCKET_ERROR when not active
		*/
        SOCKET sock;
		/* default send/recv address */
        SocketAddress defaultAddress;

    public:
		/* constructors and distrutors */
        BsdSocket();
        BsdSocket(int family, int type, int protocol, int paramPort, bool blocking, int minBufferSize);
        ~BsdSocket();


    protected:

		/* socket call */
        bool socket(int family, int type, int protocol, int paramPort, bool blocking, int minBufferSize);
		/* close call */
        void close();

		/* misc functions for setting socket parameters */
        void setMinimumBufferSize(int minBufferSize);
        void setBlockingMode(bool blocking);

		/* function for retriving local address */
        void getLocalAddress(SocketAddress * saPtr);


		// callback when data arrives
		virtual void dataArrivalCallback() {}
    public:

		/* send */
        inline int send(char * buffer, int length)
        {
            return (sendto(sock, buffer, length, 0, defaultAddress.getAddrPtr(), defaultAddress.getSize()) == SOCKET_ERROR);
        }

		/* send */
        inline int sendTo(char * buffer, int length, SocketAddress * addressPtr)
        {
            return (sendto(sock, buffer, length, 0, addressPtr->getAddrPtr(), addressPtr->getSize()) == SOCKET_ERROR);
        }

		/* recv */
        inline bool recv (char * buffer, int * length)
        {
            int temp_len;
            if ((temp_len = ::recv(sock, buffer, *length, 0)) > 0) {
                *length = temp_len;
                return true;
            }
            return false;
        }

        /* recv */
		inline bool recvFrom (char * buffer, int * length, SocketAddress * addressPtr)
        {
			int temp_len;
			if ((temp_len = recvfrom(sock, buffer, *length, 0, addressPtr->getAddrPtr(), addressPtr->getSizePtr())) > 0) {
				*length = temp_len;
				return true;
			}
			return false;
        }



    private:
		/* sockets management stuff */
        static StaticArray<BsdSocket*, FD_SETSIZE> socketsList;
        static SOCKET ndfs;
        static fd_set fdList;
        static fd_set tempFdList;
		static void calcNdfs();

	protected:
		/* big buffer for receiving data */
        static char bigRecvBuffer[BIG_RECV_BUFFER_SIZE];

    public:
        static inline bool step(int secs, int ms);
        static void initialize();
        static void terminate();

    };

};
