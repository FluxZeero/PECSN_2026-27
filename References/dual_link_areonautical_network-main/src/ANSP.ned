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

package dual_link_aeronautical_network;

//
// TODO auto-generated module
//

simple TrafficGen {
    parameters:
        volatile int size @unit(B) = default(intuniform(50B, 250B, 0));
        volatile double interGenerationTime @unit(s) = default(uniform(0s, 60s, 1)); // "T"
        volatile double linkRV = uniform(0, 1, 2);
        // if uniform <= p
        // 		satellite link is used
        // else
        // 		terrestrial link is used
        double p = default(0.5);
    gates:
        output out;
}


simple Scheduler {
    parameters:
        bool alternatingMsg = default(false);
        double Bt @unit(bps) = default(32kbps);
        double Bs @unit(bps) = 2 * Bt;
        @signal[queueSizeSignal](type="long");
        @statistic[queueSize](title="Queue Size"; source="queueSizeSignal"; record=vector,mean,stats,histogram);
    gates:
        input in[];  // Vettore di ingressi (da ogni TrafficGen)
        output satelliteRadioLink[]; 
        output terrestialRadioLink[]; 
}

module ANSP
{
    parameters:
        int numberOfAircrafts = default(10); //"N"
        double Dt @unit(s) = default(100ms);
        double Ds @unit(s) = 2 * Dt;
        @display("i=device/server;bgb=400,300");
    gates:
        output satelliteRadioLink[numberOfAircrafts];
        output terrestialRadioLink[numberOfAircrafts];
    submodules:
        traffic[numberOfAircrafts] : TrafficGen {
            @display("p=50,50,c,40");
        }
        
        scheduler: Scheduler {
            @display("p=250,150;i=block/dispatch;is=l");
            gates:
                in[parent.numberOfAircrafts];
                satelliteRadioLink[parent.numberOfAircrafts]; 
        		terrestialRadioLink[parent.numberOfAircrafts]; 
        }
        
    connections:
        
        for i=0..numberOfAircrafts-1 {
            traffic[i].out --> { @display("ls=gray,1"); } --> scheduler.in[i];
        	scheduler.satelliteRadioLink[i] --> { delay = parent.Ds; } --> satelliteRadioLink[i];
 			scheduler.terrestialRadioLink[i] --> { delay = parent.Dt; } --> terrestialRadioLink[i]; 
        }
}

