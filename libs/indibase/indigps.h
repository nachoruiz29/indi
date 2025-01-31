/*******************************************************************************
  Copyright(c) 2015 Jasem Mutlaq. All rights reserved.

  INDI GPS Device Class

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.

  The full GNU General Public License is included in this distribution in the
  file called LICENSE.
*******************************************************************************/

#pragma once

#include "defaultdevice.h"

/**
 * \class GPS
   \brief Class to provide general functionality of a GPS device.

   The GPS provides a simple interface for GPS devices. It reports time in INDI standard property TIME_UTC. Location is reported in INDI standard property GEOGRAPHIC_COORD
   Only one function is called by the INDI framework to update GPS data (updateGPS()). If the data is valid, it is sent to the client. If GPS data is not ready yet, updateGPS will
   be called every second until the data becomes available and then INDI sends the data to the client.

   updateGPS() is called upon successful connection and whenever the client requests a data refresh.

   \example GPS Simulator is available under Auxiliary drivers as a sample implementation of GPS
   \e IMPORTANT: GEOGRAPHIC_COORD stores latitude and longitude in INDI specific format, refer to <a href="http://indilib.org/develop/developer-manual/101-standard-properties.html">INDI Standard Properties</a> for details.

\author Jasem Mutlaq
*/
namespace INDI
{

class GPS : public DefaultDevice
{
    public:
        enum GPSLocation
        {
            LOCATION_LATITUDE,
            LOCATION_LONGITUDE,
            LOCATION_ELEVATION
        };

        GPS() = default;
        virtual ~GPS() = default;

        virtual bool initProperties() override;
        virtual bool updateProperties() override;
        virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) override;
        virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;

    protected:
        /**
             * @brief updateGPS Retrieve Location & Time from GPS. Update LocationNP & TimeTP properties (value and state) without sending them to the client (i.e. IDSetXXX).
             * @return Return overall state. The state should be IPS_OK if data is valid. IPS_BUSY if GPS fix is in progress. IPS_ALERT is there is an error. The clients will only accept values with IPS_OK state.
             */
        virtual IPState updateGPS();

        /**
             * @brief TimerHit Keep calling updateGPS() until it is successfull, if it fails upon first connection.
             */
        virtual void TimerHit() override;

        /**
         * @brief saveConfigItems Save refresh period
         * @param fp pointer to config file
         * @return True if all is OK
         */
        virtual bool saveConfigItems(FILE *fp) override;

        //  A number vector that stores latitude, longitude and altitude.
        INDI::PropertyNumber LocationNP {3};

        // UTC and UTC Offset
        INDI::PropertyText TimeTP {2};

        // Refresh data
        INDI::PropertySwitch RefreshSP {1};

        // Refresh Period
        INDI::PropertyNumber PeriodNP {1};

        int timerID = -1;
};
}
