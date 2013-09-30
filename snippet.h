void evalBallots(std::ostream& w, int omegaBallot[MAX_BALLOTS][MAX_CANDIDATES], char candidateName[MAX_CANDIDATES][MAX_CHARS], int numArr[])
{
	//numArr contains {numCanidates, numBallots}
	bool output = false;
	const int numCandidates = numArr[0];
	const int numBallots = numArr[1];
	int tally[MAX_CANDIDATES] = {0};
	bool isTie;
	std::map<int, int> roundBallots;
	std::map<int, int> losers;
	int min,winThreshold,index, max;
	std::map<int, int>::iterator it = roundBallots.begin();
	int r, c;
	r = 0;
	for (c = 0; c < numBallots; c++) {
		roundBallots[c] = omegaBallot[c][0];
		if (output) {
			cout << "ARRAY: Voter: " << c << " Candidate: " <<omegaBallot[c][0]<< endl;
			cout << "\tPEAK: Voter: " << c << " Candidate: " <<omegaBallot[c][1]<< endl;
		}
	}
	if (output) {
		for (it = roundBallots.begin(); it != roundBallots.end(); it++) {
			cout << "MAP: Voter: " << it->first << " Candidate: " << it->second << endl;
		}
	}
	//Break out of this loop when we find winner/draw
	int round = 0;
	int numCandidatesLeft = numCandidates;
	isTie = true;
	if(numBallots%2==0)
		winThreshold = numBallots/2;
	else
		winThreshold = (numBallots+1)/2;

	while (1) {
		//iterate over current round, tally up votes
		for(it = roundBallots.begin(); it != roundBallots.end(); it++) {
			if(round>0){
				if(losers.find(it->second) == losers.end())
					tally[it->second]+=1;
			}
			else{
				tally[it->second]+=1;
			}
			// Repeat if you also want to iterate through the second map.
		}

		//print out tally for current round
		if (output) {
			cout << "TALLY FOR ROUND: " << round << endl;
			for (index = 1; index <= numCandidates; index++)
				cout << candidateName[index] << " \tTally: " << tally[index] << endl;
		}
		//check for winner or draw
		min = numBallots+1;
		max = 0;
		//check for winner by iterating through tally
		for(index=1;index<=numCandidates;index++){
			if(tally[index]>winThreshold){
				w << candidateName[index] << endl;
				return;
			}
			//calc min a.k.a losers of current round
			if(tally[index]<min && tally[index] > 0)
				min = tally[index];
			//calc max
			if(tally[index]>max )
				max = tally[index];
		}
		// if min==max, by squeeze theorem, every active voter has same vote
		if(min==max){
			//w << "Winner: " << endl;
			for(index=1;index<=numCandidates;index++){
				if(tally[index]==max)
					w  << candidateName[index] << endl;
			}
			return;
		}
		//record which candidates are losers
		for (index = 1; index <=numCandidates; index++) {
			if(tally[index] == min || tally[index] == 0){
				losers[index] = -1;
				numCandidatesLeft--;
				if (output)cout << "ADDED TO LOSER: " << index << endl;
			}
		}
		if (output) {
			cout << "\nCurrent Ballot for Round" << endl;

			for (it = roundBallots.begin(); it != roundBallots.end(); it++)
				cout << "Voter: "<< it->first <<" choice: " << it->second << endl;
			if (it->first==8)
				cout << omegaBallot[it->first][round+1] << endl;

			cout << endl;
		}
		//traverse ballot, modify ONLY those who voted for a losing candidate
		for (it = roundBallots.begin(); it != roundBallots.end(); it++) {
			//voter's choice was a loser, get his next choice
			if(losers.find(it->second) != losers.end()) {
				//cout << "^ was in loser?"<<endl;
				if (output) {
					cout << "Lost this Round: " << it->second << " - Voter: " << it->first << endl;
				}
				bool isFound = false;
				//check row's columns until first non-loser candidate
				for(c=0;c<numCandidates && !isFound;c++){

					//pass if winner and set
					if(losers.find(omegaBallot[it->first][c]) == losers.end()){
						isFound=true;
						if(output)cout << "Voter " <<it->first << "'s vote was changed from " << roundBallots[it->first]<< " to " << omegaBallot[it->first][c] << endl;
						roundBallots[it->first] = omegaBallot[it->first][c];
					}
				}

			}
		}

		if (output) {
			cout << "\nModified Ballot for NEXT Round" << endl;
			for (it = roundBallots.begin(); it != roundBallots.end(); it++)
				cout << "Voter: "<< it->first <<" choice: " << it->second << endl;
			cout << endl;
		}
		round++;
		//zero out all candidates
		for (index = 1; index <= numCandidates; index++)
			tally[index] = 0;
	}
}
