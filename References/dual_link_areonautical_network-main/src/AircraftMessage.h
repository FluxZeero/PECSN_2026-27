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

#ifndef AIRCRAFTMESSAGE_H_
#define AIRCRAFTMESSAGE_H_

#include <omnetpp.h>

using namespace omnetpp;

class AircraftMessage : public cMessage {
private:
    int aircraftId;
    // 0 satellite
    // 1 terrestrial
    int link;
    // msg size
    int size;
    simtime_t generationTime; //timestamp
public:
    AircraftMessage(const char *name=nullptr, int aircraftId=0, int link=0, int size=0);
    std::string getString() const;
    void setSize(int size);
    void setAircraftId(int aircraftId);
    void setLink(int link);
    int getAircraftId();
    int getLink();
    simtime_t getGenerationTime();
    int getSize();
};

#endif /* AIRCRAFTMESSAGE_H_ */
