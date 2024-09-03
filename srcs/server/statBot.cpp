#include "../../includes/Helpers.hpp"

// Comparator for sorting the vector of pairs in descending order of counts
bool comparePairs(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
    return a.second > b.second;
}

std::vector<std::pair<std::string, int> > getTopContributors( Channel& channel ) {
	// Convert the map to a vector of pairs
	size_t	  totalmessages = 0;
    std::vector<std::pair<std::string, int> > contributors;
	std::map<std::string, int> messagesCount = channel.statBot.messagesCount; 
    for (std::map<std::string, int>::iterator it = messagesCount.begin(); it != messagesCount.end(); ++it){
		contributors.push_back(*it);
		totalmessages += (*it).second;
	}

// Sort the vector by the count in descending order
    std::sort(contributors.begin(), contributors.end(), comparePairs);

   // Add the header as the first element
    std::vector<std::pair<std::string, int> > result;
    result.push_back(std::make_pair(":::: Channel Age :::", -1)); // -1 signifies header
    result.push_back(std::make_pair(channel.getChannelAge(), -1)); 
    result.push_back(std::make_pair(":::: Totale Messages :::", -1));
    result.push_back(std::make_pair(intToString(totalmessages), -1));
    result.push_back(std::make_pair(":::: Messages Leaderboard :::", -1));

    // Add the sorted contributors with rankings
    for (size_t i = 0; i < contributors.size(); ++i) {
        result.push_back(std::make_pair("-(" + intToString(i + 1) + ") " +
											 contributors[i].first + " " +
									 intToString(contributors[i].second) +
									" messages", contributors[i].second));
    }

    return (result);
}

void	ircBot( Channel& channel ){

	std::vector<std::pair<std::string, int> >	data = getTopContributors(channel);
	// data.push_back("StatBot reporting for duty!");
	// data.push_back("test1");
	// data.push_back("test2");

	for (size_t i = 0; i < data.size(); ++i){
		std::string message = ":statBot PRIVMSG " + channel.getChannelName() + " " + data[i].first;
		//Operators
		for (size_t j = 0; j < channel.getChannelOperators().size() ; ++j){
				if (!messageToClient(*channel.getChannelOperators()[j], message)){
					std::cout << RED << "Send failure" << RESET << std::endl;
				}
		}

		//Clients
		for (size_t j = 0; j < channel.getChannelClients().size() ; ++j){
				if (!messageToClient(*channel.getChannelClients()[j], message)){
					std::cout << RED << "Send failure" << RESET << std::endl;
				}
		}
	}

}

