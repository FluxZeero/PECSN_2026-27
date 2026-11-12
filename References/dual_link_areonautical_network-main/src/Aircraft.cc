//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
#include "AircraftMessage.h"

#include "Aircraft.h"

Define_Module(Aircraft);

void Aircraft::initialize()
{
    e2eDelayTotalSignal = registerSignal("e2eDelayTotalSignal");
    e2eDelaySatSignal = registerSignal("e2eDelaySatSignal");
    e2eDelayTerrSignal = registerSignal("e2eDelayTerrSignal");
}

void Aircraft::handleMessage(cMessage *msg)
{

        AircraftMessage *airMsg = check_and_cast<AircraftMessage *>(msg);

        EV_INFO << "msg received " << airMsg->getString() << endl;

        // delay calculation
        simtime_t delay = simTime() - airMsg->getGenerationTime();

        EV_INFO << "E2E-Delay: " << delay << endl;

        emit(e2eDelayTotalSignal, delay);

        if (airMsg->getLink() == 0) { // Satellite
                emit(e2eDelaySatSignal, delay);
        } else if (airMsg->getLink() == 1) { // Terrestrial
                emit(e2eDelayTerrSignal, delay);
        }


        delete airMsg;
}
