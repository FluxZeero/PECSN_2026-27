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

AircraftMessage::AircraftMessage(const char *name, int aircraftId, int link, int size)
    : cMessage(name), aircraftId(aircraftId), link(link), size(size)
{
    this->generationTime = simTime();
}

void AircraftMessage::setAircraftId(int aircraftId) {
    this->aircraftId = aircraftId;
}

void AircraftMessage::setLink(int link) {
    this->link = link;
}

void AircraftMessage::setSize(int size) {
    this->size = size;
}

int AircraftMessage::getAircraftId() {
    return this->aircraftId;
}

int AircraftMessage::getLink() {
    return this->link;
}

simtime_t AircraftMessage::getGenerationTime() {
    return this->generationTime;
}


int AircraftMessage::getSize() {
    return this->size;
}

std::string AircraftMessage::getString() const {
    std::string linkName;
    if (this->link == 0) {
        linkName = "Satellite";
    } else if (this->link == 1) {
        linkName = "Terrestrial";
    } else {
        linkName = "Unknown(" + std::to_string(this->link) + ")";
    }

    return "[AircraftMessage] ID: " + std::to_string(this->aircraftId) +
           " | Link: " + linkName + " | Size: " + std::to_string(this->size) + " bytes";
}

