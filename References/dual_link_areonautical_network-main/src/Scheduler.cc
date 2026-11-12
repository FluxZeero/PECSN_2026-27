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

#include "Scheduler.h"


Define_Module(Scheduler);

void Scheduler::initialize()
{
    messageQueue_ = new cQueue();
    queueSizeSignal_ = registerSignal("queueSizeSignal");
    emit(queueSizeSignal_, (long)0);
    satelliteFree_ = true;
    terrestrialFree_ = true;

    // timer for satellite link
    sendingTimer_[0] = new cMessage("msg-transmitted",0);
    // timer for terrestrial link
    sendingTimer_[1] = new cMessage("msg-transmitted",1);
    // buffered msg, the one being sent of the satellite link
    msgUnderSend_[0] = nullptr;
    // buffered msg, the one being sent of the terrestrial link
    msgUnderSend_[1] = nullptr;

    // counter used of alternating jobs configuration
    counter_ = -1;
    alternatingMsg_ = par("alternatingMsg");
}



// returns true if a msg can be sent
// if its link is free
bool Scheduler::freeToSend_(AircraftMessage* msg) {
    int link = msg->getLink();
    return (link == 0 && satelliteFree_) || (link == 1 && terrestrialFree_);
}

// sets the boolean of the free channel to false because of the new msg that will be sent
void Scheduler::occupyLink_(AircraftMessage* msg) {
    if(msg == nullptr) return;
    int link = msg->getLink();
    if (link == 0)
        satelliteFree_ = false;
    else
        terrestrialFree_ = false;
}

// sets the boolean of the free channel to true after a msg has been sent
void Scheduler::freeUpLink_(AircraftMessage* msg) {
    if(msg == nullptr) return;
    int link = msg->getLink();
    if (link == 0)
        satelliteFree_ = true;
    else
        terrestrialFree_ = true;
}


void Scheduler::handleMessage(cMessage *msg)
{
    // received a msg from the traffic gen
    if(!msg->isSelfMessage()) {

        // used for validation
        counter_++;

        AircraftMessage *airMsg = check_and_cast<AircraftMessage *>(msg);

        // sets the msgs on alternating channel
        // used for validation
        if (alternatingMsg_)
            airMsg->setLink(counter_%2);

        EV_INFO << "Received message from TrafficGen[" << airMsg->getAircraftId() << "]" << "for link " << counter_%2 << " " << endl;

        // Insert into queue
        messageQueue_->insert(airMsg);

        emit(queueSizeSignal_, (long)messageQueue_->getLength());
        EV_INFO << "Message queued. Queue length: " << messageQueue_->getLength() << endl;
    }

    // the transmission timer has expired (the transmission delay has passed), the msg has to be sent
    if(!strcmp(msg->getName(), "msg-transmitted") && msg->isSelfMessage()){

        // The kind of the timer is used to know on which link was the msg being processed
        int msgLink = msg->getKind();

        AircraftMessage *airMsg = check_and_cast<AircraftMessage *>(msgUnderSend_[msgLink]);
        if (msgLink == 0) {
            send(airMsg, "satelliteRadioLink", airMsg->getAircraftId());
        }
        else if (msgLink == 1) {
            send(airMsg, "terrestialRadioLink", airMsg->getAircraftId());
        }

        // the msg link can now be freed as the transmission delay has passed
        freeUpLink_(msgUnderSend_[msgLink]);

        EV_INFO << "msg transmitted " << msgUnderSend_[msgLink]->getString() << endl;
        msgUnderSend_[msgLink] = nullptr;

    }

    // The for loop has 2 iterations as it's possible that both link are free and so 2 new msgs have to be sent
    // It could happen that at the head of the queue  a satellite msg blocks a terrestrial msg even if the terrestrial link is free.
    // in this case once the satellite link is freed up, both msgs must be sent simultaneously
    for (int i = 0; i < 2; i++){

        if(messageQueue_->isEmpty())
            return;

        // if there's a message to be sent check the link and send it
        AircraftMessage* nextMsg = (AircraftMessage*) messageQueue_->front();
        int link = nextMsg->getLink();
        if (!freeToSend_(nextMsg))
            return;

        //ready to send first message in queue
        nextMsg = (AircraftMessage*)messageQueue_->pop();
        emit(queueSizeSignal_, (long)messageQueue_->getLength());
        // calculate trasmission delay
        double sizeInBits = nextMsg->getSize() * 8.0;
        double bandwidth = (link == 0) ? par("Bs").doubleValue() : par("Bt").doubleValue();
        double transmissionDelay = sizeInBits / bandwidth;
        EV_INFO << "Starting transmission. Delay: " << transmissionDelay << endl;
        EV_INFO << "Queue length: " << messageQueue_->getLength() << endl;
        msgUnderSend_[link] = nextMsg;
        occupyLink_(nextMsg);
        //set timer for freeing the link after transmission delay
        scheduleAt(simTime() + transmissionDelay, sendingTimer_[link]);

    }
    return;
}


Scheduler::~Scheduler() {

    cancelAndDelete(sendingTimer_[0]);
    cancelAndDelete(sendingTimer_[1]);


    if (msgUnderSend_[0] != nullptr) {
        delete msgUnderSend_[0];
    }
    if (msgUnderSend_[1] != nullptr) {
        delete msgUnderSend_[1];
    }


    delete messageQueue_;
}
