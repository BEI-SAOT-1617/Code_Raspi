#include "planManager.h"
#include "plan.h"

#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <time.h>
#include <iostream>

#include "genericInstruction.h"

using namespace std;


PlanManager::PlanManager(){
	this->nPlan = 0;
	this->ptInstruction = 0;
}
/* Merci Felix!! :) */


void PlanManager::executePlan(){


if (nPlan > 0)
{
	Plan P = this->Plans[indexPlan];
	GenericInstruction* currentInst;

	CameraController* myCameraController;
	AttitudeController* myAttitudeController;

	bool instructionCatched, jumpInstruction;
	//for (int i = 0 ; i < P.getnInstructions(); i++){

		currentInst = P.getInstruction(ptInstruction);
		instructionCatched = false;
		jumpInstruction = false;

		//while(wt){

		time_t t = time(0);
		struct tm * now = localtime(&t);

		int group = currentInst->getIndex();
		for (int i = 0; i < 50; i++) {
			if (group == bannedInstructions[i]) jumpInstruction = true;
		}
		//if (jumpInstruction == false) {
			if ((now->tm_hour == currentInst->getHour()) & (now->tm_min == currentInst->getMin()) & (now->tm_sec == currentInst->getSec())) {
			
				if (currentInst->getType() == 'p') {

					int exposure = currentInst->getExposure();
					string photoName = currentInst->getPhotoName();
					
					exposure = 500;
					cout << endl << "New photo! Smile!" << endl;
					myCameraController->photoShoot(photoName, exposure);

				}
				else if (currentInst->getType() == 'a') {

					int yaw = currentInst->getYaw();
					int pitch = currentInst->getPitch();
					int roll = currentInst->getRoll();

					cout << endl << "New attitude change!!" << endl;
					myAttitudeController->attitudeChange(yaw, pitch, roll); // roll(not used)
				}

				ptInstruction++;
				// MODIF : test >=
				if (ptInstruction >= P.getnInstructions()){ 
				ptInstruction = 0;
				indexPlan++;
				indexPlan=indexPlan%5;
				nPlan--;
				}
				
				instructionCatched = false;
			}
		//}
		//}
	//}

	}
}

Plan PlanManager::generatePlan(const char* filepath){
	
	unsigned int version;
	unsigned int num_plan;
	bool existnewPlan = true;

	string s = filepath;
	version = s[4]-'0';
	num_plan = s[6]-'0';
	
	Plan myPlan(version, num_plan);
	myPlan.loadPlan(filepath);
				
	//Plans[nPlan].printPlan();

	//cout << "version"<< version << endl;
	//cout << "num_plan"<< num_plan<< endl;

	for (int i = 0 ; i < nPlan; i++){
		if (Plans[(i+indexPlan)%5].getID()==num_plan){
				if (Plans[(i+indexPlan)%5].getVersion() < version){
					Plans[(i+indexPlan)%5]= myPlan;
				}
		existnewPlan=false;
		}
	}
	if (existnewPlan){
			Plans[(nPlan+indexPlan)%5]= myPlan;
			nPlan++;
	}

}


