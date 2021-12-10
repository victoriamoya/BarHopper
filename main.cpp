#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <utility>
#include <map>
#include <queue>
#include <vector>
#include <set>
#include <iostream>
#include <cmath>
#include <random>
#include <utility>
#include <iomanip>
#include <list>

using namespace std;
using std::setprecision;
class Bar
{
public:
    string name;
    double rating;
    pair<double, double> coordinates;
    Bar()
    {
        name = "";
        rating = 0.0;
        coordinates.first = 0.0;
        coordinates.second = 0.0;
    }
    Bar(string name, double rating, pair<double, double> coordinates)
    {
        this->name = name;
        this->rating = rating;
        this->coordinates = coordinates;
    }
    bool operator==(const Bar& compare) const
    {
        if (compare.name.compare(this->name) != 0)
            return false;
        else if (compare.rating != this->rating)
            return false;
        else if (compare.coordinates != this->coordinates)
            return false;
        else
            return true;
    }
    bool operator<(const Bar& compare) const
    {
        if (this->coordinates.first < compare.coordinates.first || this->coordinates.second < compare.coordinates.second)
            return true;
        else
            return false;
    }
    void setBar(string name, double rating, pair<double, double>)
    {
        this->name = name;
        this->rating = rating;
        this->coordinates = coordinates;
    }
};
class BarGraph
{
private:
    map<Bar, vector<pair<Bar, double>>> allBars;
    double walkingDist;
public:
    BarGraph(double walkingDist);
    void insert(string name, double rating, pair<double, double> coord);
    double findDist(const Bar& bar1, const Bar& bar2);
    double findDist(const Bar& bar1, pair<double, double> coordinates);
    vector<Bar> barsCloseUser(pair<double, double> coordinates, double range);
    map<Bar, pair<Bar, double>> Djikstra(Bar& start);
    set<Bar> BFS(Bar& start);
    double FindEdgeLength(Bar& bar1, Bar& bar2);
    vector<Bar> optionB( Bar& start,  Bar& end);
    vector<Bar> optionA(const Bar& bar);
    void printBars();
};
BarGraph::BarGraph(double walkingDist)
{
    this->walkingDist = walkingDist;
}
void BarGraph::insert(string name, double rating, pair<double, double> coord)
{
    //create then add the bar
    Bar bar = Bar(name, rating, coord);
    //first add the bar
    allBars[bar];
    //go through and add the bar to the vectors if within radius
    for (auto iter = allBars.begin(); iter != allBars.end(); iter++)
    {
        double dist = findDist(iter->first, bar);
        if (!(iter->first == bar) && dist <= walkingDist)
        {
            allBars[bar].push_back(make_pair(iter->first, dist));
            iter->second.push_back(make_pair(bar, dist));
        }
    }
}
double BarGraph::findDist(const Bar& bar1, const Bar& bar2)
{
    double val1 = pow(bar1.coordinates.first - bar2.coordinates.first, 2);
    double val2 = pow(bar1.coordinates.second - bar2.coordinates.second, 2);
    return sqrt(val1 + val2);
}
double BarGraph::findDist(const Bar& bar1, pair<double, double> coordinates)
{
    double val1 = pow(bar1.coordinates.first - coordinates.first, 2);
    double val2 = pow(bar1.coordinates.second - coordinates.second, 2);
    return sqrt(val1 + val2);
}

vector<Bar> BarGraph::barsCloseUser(pair<double, double> coordinates, double range)
{
    vector<Bar> closeBars;
    for (auto iter = allBars.begin(); iter != allBars.end(); iter++)
    {
        if (findDist(iter->first, coordinates) <= range)
            closeBars.push_back(iter->first);
    }
    return closeBars;
}
//bool operator>(const Bar& lhs, const Bar& rhs){return lhs.distance > rhs.distance;}
map<Bar, pair<Bar, double>> BarGraph::Djikstra(Bar& start)
{
    //initialize a set of visited and not yet visited bars
    set<Bar> visited = {};
    set<Bar> notVisited;
    //initialize a map with all the bars, and their previous node and the distance to the previous node
    map<Bar, pair<Bar, double>> djik;
    Bar bar = Bar();
    //cout << "Enter djikstra" << endl;
    for (auto iter = allBars.begin(); iter != allBars.end(); iter++)
    {
        djik[iter->first] = make_pair(bar, 99999);
        notVisited.insert(iter->first);
    }
    //go through the list of unvisited until there are none left
    Bar currentBar = start;
    visited.insert(start);
    notVisited.erase(start);
    djik[start] = make_pair(currentBar, 0);

    while (!(notVisited.empty()) && djik[currentBar].second < 999)
    {
        for (int i = 0; i < allBars[currentBar].size(); i++)
        {
            //relaxation method
            Bar compare = allBars[currentBar][i].first;
            double edgeLength = allBars[currentBar][i].second;
            if (visited.find(compare) == visited.end() && djik[compare].second >= djik[currentBar].second + edgeLength)
            {
                //update previous bar and distance
                djik[compare].first = currentBar;
                djik[compare].second = djik[currentBar].second + edgeLength;
            }
        }
        //add current bar to visited and remove it from not visited
        visited.insert(currentBar);
        notVisited.erase(currentBar);
        if (notVisited.empty())
            break;
        //pick next non-visited bar based on one with min total distance
        Bar min = *notVisited.begin();
        for (auto iter = notVisited.begin(); iter != notVisited.end(); iter++)
        {
            if (djik[*iter].second < djik[min].second && visited.find(*iter) == visited.end())
            {
                //cout << "min" << endl;
                min = *iter;
            }
        }
        currentBar = min;
        if (djik[currentBar].second >= 900)
            break;
    }
    //cout << " done w dijkstra" << endl;
    return djik;
}

set<Bar> BarGraph::BFS(Bar& start)
{

    list<Bar> queue;
    set<Bar> barSet;

    barSet.insert(start);
    queue.push_back(start);
    while(!queue.empty())
    {
        // Dequeue from queue and print
        Bar curr = queue.front();
        //cout << src << " ";
        queue.pop_front();
        // if a adjacent has not been visited then visit it and enqueue it
        for (auto p  : allBars[curr]) {
            if (barSet.find(p.first) == barSet.end())
            {
                barSet.insert(p.first);
                queue.push_back(p.first);
            }
        }
    }
    return barSet;
}

double BarGraph::FindEdgeLength(Bar& bar1, Bar& bar2)
{
    for (int i = 0; i < allBars[bar1].size(); i++)
    {
        if (allBars[bar1][i].first == bar2)
            return allBars[bar1][i].second;
    }
    return -1;
}
void BarGraph::printBars()
{
    for (auto iter = allBars.begin(); iter != allBars.end(); iter++)
    {
        cout << iter->first.name << " Rating: " << iter->first.rating << endl;
        cout << "Neighbors: ";
        for (int i = 0; i < iter->second.size(); i++)
        {
            cout << iter->second[i].first.name << " ";
        }
        cout << endl;
    }
}
double generateDouble(int min, int max) {
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<double> bounds(min, max);
    setprecision(4);
    return bounds(eng);
}

string randName()
{
    vector<string> barList = { "Senor Frogs", "Treehouse", "Pots", "Warehouse","Clydes","El Santo","Space","The Hangar","FoxHole","Gramercy","Baas","E11even","LIV","Wynwood Factory","El Patio","The Oasis","Story Nightclub","Basement","Las Rosas","Flanigans","Racket","Komodo","Bodega","Nikki Beach","W Hotel","Recess","Exchange","Heart","Mokai","Studio 23","Wall Lounge","Mynt Lounge","Clevelander","Ora","Knights Pub", "Hillstone", "The Tipsy Cow", "Bar", "Resturant", "Club", "Tap42", "Sway", "Rosario", "The Wharf", "Icebar","Fling", "Mango's Tropical Cafe","The range","White Buffalo","Downtown Fats","Fat Daddys","JJs Tavern","Tatu","Grog","Salty Dog Saloon","Balls","The Lab","The Loft","Barcade","Simons","Billiards","Cafe Risque","Social","Rowdy Reptiles","Original American Kitchen", "Dragonfly", "Aman Bar", "Earl's","Cheesecake Factory", "Jim's Tavern", "Hole in the Wall", "The Dirty South", "The Streets","PO" };
    int barListIndex = rand() % barList.size();
    return barList[barListIndex];
}
string randNameUF()
{
    vector<string> barList = { "The Range","White Buffalo","Downtown Fats","Fat Daddys","JJs Tavern","Tatu","Grog","Salty Dog Saloon","Balls","The Lab","The Loft","Barcade","Simons","Billiards","Cafe Risque","Social","Rowdy Reptiles","Original American Kitchen", "Dragonfly", "Mothers Irish Pub", "The Top", "V pizza", "Boca Fiesta", "The Backyard"};
    int barListIndex = rand() % barList.size();
    return barList[barListIndex];
}
double randRating()
{
    double rating = generateDouble(1, 5);

    return (int)rating;
}

pair<double,double> generateOneinClusters(pair<double,double>location) {

    double xcoor = location.first + generateDouble(-5, 5);
    double ycoor = location.second + generateDouble(-5, 5);

    pair <double,double> xyPair;
    xyPair = {xcoor,ycoor};

    return xyPair;
}
pair<double,double> generateOneinRandomBars() {

    double xcoor =  generateDouble(-500, 500);
    double ycoor =  generateDouble(-500, 500);

    pair <double,double> xyPair;
    xyPair = {xcoor,ycoor};

    return xyPair;
}
vector<Bar> BarGraph::optionB(Bar& start, Bar& end)
{
    map<Bar, pair<Bar, double>> djik = Djikstra(start);
    vector<Bar> barPath;
    barPath.push_back(end);
    Bar currBar = end;
    if (djik.find(end) == djik.end())
    {
        barPath.push_back(start);
    }
    else
    {
        while(!(currBar == start))
        {
            currBar = djik[currBar].first;
            barPath.push_back(currBar);
        }
    }
    return barPath;
}

vector<Bar> BarGraph::optionA(const Bar& bar)
{
    set<Bar> visited;
    vector<Bar> path = {bar};
    Bar currBar = bar;
    while(currBar.name != "")
    {
        double highestRatio = 0;
        Bar highest = Bar();
        for (int i = 0; i < allBars[currBar].size(); i++)
        {
            if (visited.find(allBars[currBar][i].first) == visited.end())
            {
                double ratingDistRatio = allBars[currBar][i].second / allBars[currBar][i].first.rating;
                if (ratingDistRatio > highestRatio)
                {
                    highestRatio = ratingDistRatio;
                    highest = allBars[currBar][i].first;
                }
            }
        }
        if (highest.name == "")
            break;
        else
        {
            visited.insert(currBar);
            path.push_back(highest);
        }
        currBar = highest;
    }
    return path;
}
int main() {
    BarGraph graph = BarGraph(3.0);
    Bar obj;

    for (int i = 0; i < 100000; i++) {
        string name = randName();
        double rating = randRating();
        pair<double, double> coor = generateOneinRandomBars();

        graph.insert(name, rating, coor);
    }


    double xLocation = 0.0;
    double yLocation = 0.0;
    pair <double,double> locationCoordinates;
    int startBar = 0;
    int endBar = 0;
    int numOfBars = 0; //for the final slide for option A
    string menuOption = "";
    double radius = 0.0;

    //cout << "Welcome to BarHooper! We have loaded " <<vector.size() << " bars"<< endl;
    cout << "Welcome to BarHooper! "<< endl;
    cout <<"Choose a bar nearby and we will show you your optimal path to get the most out of your night!"<<endl;
    cout <<"Our vast library contains over a 100,000 bars!"<<endl;
    cout<<"\n";

    //xLocation
    cout<< "Enter x location between -500 and 500: ";
    cin>> xLocation;

    //yLocation
    cout<< "Enter y location between -500 and 500: ";
    cin>> yLocation;

if(xLocation == 29 && yLocation == 82) {
    for (int y = 0; y < 24; y++) {
        string name = randNameUF();
        double rating = randRating();
        pair<double, double> coor = generateOneinClusters({xLocation,yLocation});

        graph.insert(name, rating, coor);
    }
}
    for (int y = 0; y < 29; y++) {
        string name = randName();
        double rating = randRating();
        pair<double, double> coor = generateOneinClusters({xLocation,yLocation});

        graph.insert(name, rating, coor);
    }

    //Radius
    cout<< "Radius between 1-250: ";
    cin>> radius;

    locationCoordinates = make_pair(xLocation,yLocation);
    cout<< "There are "<< graph.barsCloseUser(locationCoordinates,radius).size() <<" many bars in your desired area and their distance from you with their ratings"<<endl;
    cout<<"\n";

    //Print out the list of bars
    vector<Bar> UserBar = graph.barsCloseUser(locationCoordinates,radius);

    cout << setfill('*') << setw(60) << "\n" << setfill(' ');
    cout<<setw(10)<<"Name"<<setw(25)<<"Rating"<<setw(20)<<"Distance"<<endl;
    for(int i = 0; i < graph.barsCloseUser(locationCoordinates,radius).size(); i++ ) {
        int stringSize = UserBar[i].name.length();
        if(i < 9) {
            cout<< i+1 <<". " <<  UserBar[i].name <<setprecision(2)<<setw(30-stringSize) <<UserBar[i].rating<<setw(19)<<(double)graph.findDist(UserBar[i],locationCoordinates)<<endl;
        }
        else {
            cout<< i+1 <<". " <<  UserBar[i].name <<setprecision(2)<<setw(29-stringSize) <<UserBar[i].rating<<setw(19)<<(double)graph.findDist(UserBar[i],locationCoordinates)<<endl;
        }
    }

    cout << setfill('*') << setw(60) << "\n" << setfill(' ');
    cout<< "To proceed you must choose one of the following options:"<<endl;
    cout<< "     Option A: You can choose the bar to start at and the number of bars you would like to visit"<<endl;
    cout<< "     Option B: You can choose the bar to start at and the bar you would like to end at"<<endl;
    cout<< "Enter A or B"<<endl;
    cin>> menuOption;
    cout<<"\n";

    if(menuOption == "A") {
        cout<< "Enter the number next to the bar you would like to start at: ";
        cin >> startBar;

        cout<<"How many bars would you like to visit? ";
        cin >> numOfBars;
        vector<Bar> optionAvec = graph.optionA(UserBar[startBar-1]);
        cout<<"Your optimal path based of your location and radius is:" <<endl;

        cout<<"\n";
        int counter = 1;
        cout << setfill('*') << setw(60) << "\n" << setfill(' ');
        cout<<setw(10)<<"Name"<<setw(25)<<"Rating"<<setw(20)<<"Distance"<<endl;
        if(numOfBars > optionAvec.size()) {
            for (int i = 0; i < optionAvec.size(); i++) {

                int stringSize = optionAvec[i].name.length();
                if (i == 1) {
                    cout << i+1 << ". " << optionAvec[i].name << setprecision(2) << setw(30 - stringSize)
                         << optionAvec[i].rating << setw(19)
                         << (double) graph.findDist(optionAvec[i], optionAvec[i-1]) << endl;
                    //cout<< counter++ <<". " <<  optionBvec[i].name <<setprecision(2)<<setw(30-stringSize) <<optionBvec[i].rating<<setw(19)<<"x: "<<optionBvec[i].coordinates.first<< "y: "<< optionBvec[i].coordinates.second<<endl;
                } else if (i < 9) {
                    cout << i + 1 << ". " << optionAvec[i].name << setprecision(2) << setw(30 - stringSize)
                         << optionAvec[i].rating << setw(19)
                         << (double) graph.findDist(optionAvec[i], optionAvec[i - 1]) << endl;
                } else {
                    cout << i + 1 << ". " << optionAvec[i].name << setprecision(2) << setw(29 - stringSize)
                         << optionAvec[i].rating << setw(19)
                         << (double) graph.findDist(optionAvec[i], optionAvec[i - 1]) << endl;
                }
            }
            cout << setfill('*') << setw(60) << "\n" << setfill(' ');
            cout<< "There are only "<< optionAvec.size() <<" bars in the current pathway!"<<endl;


        } else {
            for (int i = 0; i < numOfBars; i++) {

                int stringSize = optionAvec[i].name.length();
                if (i == 1) {
                    cout << i << ". " << optionAvec[i].name << setprecision(2) << setw(30 - stringSize)
                         << optionAvec[i].rating << setw(19)
                         << (double) graph.findDist(optionAvec[i], optionAvec[i+1]) << endl;
                    //cout<< counter++ <<". " <<  optionBvec[i].name <<setprecision(2)<<setw(30-stringSize) <<optionBvec[i].rating<<setw(19)<<"x: "<<optionBvec[i].coordinates.first<< "y: "<< optionBvec[i].coordinates.second<<endl;
                } else if (i < 9) {
                    cout << i + 1 << ". " << optionAvec[i].name << setprecision(2) << setw(30 - stringSize)
                         << optionAvec[i].rating << setw(19)
                         << (double) graph.findDist(optionAvec[i], optionAvec[i + 1]) << endl;
                } else {
                    cout << i + 1 << ". " << optionAvec[i].name << setprecision(2) << setw(29 - stringSize)
                         << optionAvec[i].rating << setw(19)
                         << (double) graph.findDist(optionAvec[i], optionAvec[i + 1]) << endl;
                }
            }
            cout << setfill('*') << setw(60) << "\n" << setfill(' ');
        }
        //this is where we print out the last list

    }
    else if(menuOption == "B") {
        cout<< "Enter the number next to the bar you would like to START at: ";
        cin >> startBar;
        //add check if the number they entered is valid
        cout<< "Enter the number next to the bar you would like to END at: ";
        cin >> endBar;
        //add check if the number they entered is valid
        vector<Bar>optionBvec = graph.optionB(UserBar[startBar-1],UserBar[endBar-1]);

        cout<<"Your optimal path based of your location and radius is:" <<endl;
        cout<<"\n";
        int counter = 1;
        cout << setfill('*') << setw(60) << "\n" << setfill(' ');
        cout<<setw(10)<<"Name"<<setw(25)<<"Rating"<<setw(20)<<"Distance"<<endl;
        for (int i = optionBvec.size()-1; 0 <= i ; i--) {

            int stringSize = optionBvec[i].name.length();
            if(counter == 1){
                cout<< counter++ <<". " <<  optionBvec[i].name <<setprecision(2)<<setw(30-stringSize) <<optionBvec[i].rating<<setw(19)<<(double)graph.findDist(optionBvec[i],locationCoordinates)<<endl;
                //cout<< counter++ <<". " <<  optionBvec[i].name <<setprecision(2)<<setw(30-stringSize) <<optionBvec[i].rating<<setw(19)<<"x: "<<optionBvec[i].coordinates.first<< "y: "<< optionBvec[i].coordinates.second<<endl;
            }
            else if(counter < 9) {
                cout<< counter++ <<". " <<  optionBvec[i].name <<setprecision(2)<<setw(30-stringSize) <<optionBvec[i].rating<<setw(19)<<(double)graph.findDist(optionBvec[i],optionBvec[i-1])<<endl;
            }
            else {
                cout<< counter+1 <<". " <<  optionBvec[i].name <<setprecision(2)<<setw(29-stringSize) <<optionBvec[i].rating<<setw(19)<<(double)graph.findDist(optionBvec[i],optionBvec[i-1])<<endl;
            }
        }
        cout << setfill('*') << setw(60) << "\n" << setfill(' ');
        //this is where we print out the last list
    }
    else {
        cout<<"Try again with a valid answer!";
        return 0;
    }

    cout<<"\n";
    cout<<"*Disclaimer: This app was intended for users 21 and older. Remember not to drink and drive. Enjoy responsibly!*"<<endl;
}
