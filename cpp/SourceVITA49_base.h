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

 * This is the component code. This file contains the child class where
 * custom functionality can be added to the component. Custom
 * functionality to the base class can be extended here. Access to
 * the ports can also be done from this class
 */

#ifndef SOURCEVITA49IMPLBASE_H
#define SOURCEVITA49IMPLBASE_H

#include <boost/thread.hpp>
#include <ossie/Resource_impl.h>

#include "port_impl.h"
#include "struct_props.h"

#define NOOP 0
#define FINISH -1
#define NORMAL 1

class SourceVITA49_base;


template < typename TargetClass >
class ProcessThread
{
    public:
        ProcessThread(TargetClass *_target, float _delay) :
            target(_target)
        {
            _mythread = 0;
            _thread_running = false;
            _udelay = (__useconds_t)(_delay * 1000000);
        };

        // kick off the thread
        void start() {
            if (_mythread == 0) {
                _thread_running = true;
                _mythread = new boost::thread(&ProcessThread::run, this);
            }
        };

        // manage calls to target's service function
        void run() {
            int state = NORMAL;
            while (_thread_running and (state != FINISH)) {
                state = target->serviceFunction();
                if (state == NOOP) usleep(_udelay);
            }
        };

        // stop thread and wait for termination
        bool release(unsigned long secs = 0, unsigned long usecs = 0) {
            _thread_running = false;
            if (_mythread)  {
                if ((secs == 0) and (usecs == 0)){
                    _mythread->join();
                } else {
                    boost::system_time waitime= boost::get_system_time() + boost::posix_time::seconds(secs) +  boost::posix_time::microseconds(usecs) ;
                    if (!_mythread->timed_join(waitime)) {
                        return 0;
                    }
                }
                delete _mythread;
                _mythread = 0;
            }
    
            return 1;
        };

        virtual ~ProcessThread(){
            if (_mythread != 0) {
                release(0);
                _mythread = 0;
            }
        };

        void updateDelay(float _delay) { _udelay = (__useconds_t)(_delay * 1000000); };
    public:
        boost::thread *_mythread;
    private:
        bool _thread_running;
        TargetClass *target;
        __useconds_t _udelay;
        boost::condition_variable _end_of_run;
        boost::mutex _eor_mutex;
};

class SourceVITA49_base : public Resource_impl
{
    friend class BULKIO_dataVITA49_In_i;
    friend class BULKIO_dataUshort_Out_i;
    friend class BULKIO_dataShort_Out_i;
    friend class BULKIO_dataChar_Out_i;
    friend class BULKIO_dataDouble_Out_i;
    friend class BULKIO_dataFloat_Out_i;
    friend class BULKIO_dataOctet_Out_i;

    public: 
        SourceVITA49_base(const char *uuid, const char *label);

        void start() throw (CF::Resource::StartError, CORBA::SystemException);

        void stop() throw (CF::Resource::StopError, CORBA::SystemException);

        CORBA::Object_ptr getPort(const char* _id) throw (CF::PortSupplier::UnknownPort, CORBA::SystemException);

        void releaseObject() throw (CF::LifeCycle::ReleaseError, CORBA::SystemException);

        void initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException);

        void loadProperties();

        virtual int serviceFunction() = 0;

    protected:
        ProcessThread<SourceVITA49_base> *serviceThread; 
        boost::mutex serviceThreadLock;  

        // Member variables exposed as properties
        std::string interface;
        attachment_override_struct attachment_override;
        connection_status_struct connection_status;
        VITA49Processing_override_struct VITA49Processing_override;
        advanced_configuration_struct advanced_configuration;

        // Ports
        BULKIO_dataVITA49_In_i *dataVITA49_in;
        BULKIO_dataChar_Out_i *dataChar_out;
        BULKIO_dataOctet_Out_i *dataOctet_out;
        BULKIO_dataShort_Out_i *dataShort_out;
        BULKIO_dataUshort_Out_i *dataUshort_out;
        BULKIO_dataFloat_Out_i *dataFloat_out;
        BULKIO_dataDouble_Out_i *dataDouble_out;
    
    private:
        void construct();

};
#endif
