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
package dual_link_aeronautical_network.simulations;
import dual_link_aeronautical_network.ANSP;
import dual_link_aeronautical_network.Aircraft;

network DualLinkAeronauticalNetwork
{
    parameters:
        int numberOfAircrafts = default(10);
        @display("bgb=860,643;bgi=maps/europe,s");
        @statistic[e2eDelayTotal](title="End-to-End Delay (Global)"; source="e2eDelayTotalSignal"; record=vector,mean,stats,histogram);
        @statistic[e2eDelaySat](title="End-to-End Delay (Satellite)"; source="e2eDelaySatSignal"; record=vector,mean,stats,histogram);
        @statistic[e2eDelayTerr](title="End-to-End Delay (Terrestrial)"; source="e2eDelayTerrSignal"; record=vector,mean,stats,histogram);

    submodules:
        ansp: ANSP {
            numberOfAircrafts = parent.numberOfAircrafts;
            @display("p=191,241;i=device/antennatower;is=vl");
        }
        aircraft[numberOfAircrafts]: Aircraft {
            @display("p=283,97,m,5,50,50;i=plane;is=n");
        }

    connections:
        for i=0..numberOfAircrafts-1 {
            ansp.satelliteRadioLink[i] --> { @display("ls=blue,2,dashed"); } --> aircraft[i].inSatellite;
            ansp.terrestialRadioLink[i] --> { @display("ls=blue,2,dashed"); } --> aircraft[i].inTerrestrial;
        }
}
