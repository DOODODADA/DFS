#include <bits/stdc++.h>
using namespace std;

//Struct define
typedef struct edge edge;
typedef struct spot spot;
typedef struct route_value route_value;
typedef pair<int, int> ipair;
typedef priority_queue<ipair, vector<ipair>, greater<ipair> > pq;

struct edge
{
    int time;
    float flow;
};

struct spot
{
    bool available;
    string name;
    int PS;
};

struct route_value
{
    vector<int> route;
    int time;
    float flow;
    int PS;
};

//Global variable
map<string, int> nti;
spot tourist_spot[100];
edge adjmatirx[100][100];
int origin, time_flow, N, target;
route_value min_route{vector<int>{}, INT_MAX, 200, 0};

//Operator overload
bool operator>(const route_value& lhs, const route_value& rhs){

    if (time_flow == 0) {
        if (lhs.time == rhs.time && lhs.PS == rhs.PS){
            int min_size = min(lhs.route.size(), rhs.route.size());
            for (int i = 0 ; i < min_size ; i++)
                if (lhs.route[i] != rhs.route[i]) return lhs.route[i] > rhs.route[i];
        }
        if (lhs.time == rhs.time) return lhs.PS < rhs.PS;
        return lhs.time > rhs.time;
    }

    if (lhs.flow == rhs.flow && lhs.PS == rhs.PS){
            int min_size = min(lhs.route.size(), rhs.route.size());
            for (int i = 0 ; i < min_size ; i++)
                if (lhs.route[i] != rhs.route[i]) return lhs.route[i] > rhs.route[i];
    }
    if (lhs.flow == rhs.flow) return lhs.PS < rhs.PS;
    return lhs.flow > rhs.flow;
}

ostream& operator<<(ostream& os, route_value& ro){
    os << ro.time << " " << ro.PS << " ";
    for (vector<int>::iterator it = ro.route.begin() ; it != ro.route.end() ; it++)
        os << *it << " ";
    return os;
}


void dfs(int index, route_value value, set<int> pass, int limit){
    if (index == target){
        if (min_route > value){
            min_route = value;
        }
        return;
    }
    if (limit == 0) return;
    for (int i = 0 ; i < N ; i++){
        if (adjmatirx[index][i].time != 0 && tourist_spot[i].available && !pass.count(i) ){
            route_value nv = value;
            nv.flow += adjmatirx[index][i].flow;
            nv.PS += tourist_spot[i].PS;
            nv.route.push_back(i);
            nv.time += adjmatirx[index][i].time;
            pass.insert(i);
            if (min_route > nv)
                dfs(i, nv, pass, limit - 1);
            pass.erase(i);
        }
    }
}


int main(void) {

    for (int i = 0 ; i < 100 ; i++) tourist_spot[i].available = false;

    cin >> N;

    //initial tourist_spot
    for (int i = 0 ; i < N ; i++) {
        int index, ps;
        string name;
        cin >> index >> name >> ps;
        nti.insert(pair<string, int>(name, index));
        tourist_spot[index].name = name;
        tourist_spot[index].PS = ps;
        tourist_spot[index].available = true;
    }

    //initial adj matrix
    for (int i = 0 ; i < N ; i++) {
        for (int j = 0 ; j < N ; j++) {
            cin >> adjmatirx[i][j].time;
        }
    }

    for (int i = 0 ; i < N ; i++) {
        for (int j = 0 ; j < N ; j++) {
            float flow;
            cin >> flow;
            adjmatirx[i][j].flow = -log2(flow);
        }
    }

    string instruction = "start";
    while(instruction != "END_OF_INSTRUCTION"){
        cin >> instruction;

        if (instruction == "SET_ORIGIN"){
            string name;
            cin >> name;
            int index = nti[name];
            if (!tourist_spot[index].available) cout << "Fail to reset the origin" << endl;
            else {
                cout << name << " is the new origin" << endl;
                origin = index;
            }
        }

        else if (instruction == "MARK_STATE"){
            string state, name;
            cin >> state;
            bool avai = (state == "OPEN") ? true : false;
            string small_state = (state == "OPEN") ? "open" : "close";
            while(getchar() != '\n'){
                cin >> name;
                int index = nti[name];
                if (!(tourist_spot[index].available != avai && (origin != index || avai != false)))
                    cout << "Fail to " << small_state << " " << name << endl;
                else tourist_spot[index].available = avai;
            }
        }
        
        else if (instruction == "OPTIMAL_PATH"){
            string name, consideration;
            cin >> name >> consideration;
            target = nti[name];

            if (origin == target){
                cout << "Optimal " << consideration << " : " << name << endl;
            }
            else if (!tourist_spot[target].available){
                cout << "No such optimal path to " << name << endl;
            }
            else{
                time_flow = (consideration == "TIME") ? 0: 1;
                min_route.flow = adjmatirx[origin][target].flow;
                min_route.time = adjmatirx[origin][target].time;
                min_route.PS = tourist_spot[target].PS;
                min_route.route = {target};
                dfs(origin, {{}, 0, 0, 0}, {}, 150);

                vector<int>::iterator it = min_route.route.begin();

                cout << "Optimal " << consideration << " : " << tourist_spot[origin].name << " -> ";
                for(it ; it != min_route.route.end() - 1 ; it++)
                    cout << tourist_spot[*it].name << " -> ";
                cout << tourist_spot[*it].name << endl;
            }
        }
        else if (instruction == "LIMITED_PATH"){
            string name, consideration;
            int limit;
            cin >> name >> consideration >> limit;
            target = nti[name];

            if (origin == target)
                cout << "Limited " << consideration << " : " << name << endl;
            else if (!tourist_spot[target].available)
                cout << "No such limited path to " << name << endl;
            else{
                time_flow = (consideration == "TIME") ? 0: 1;
                min_route.flow = adjmatirx[origin][target].flow;
                min_route.time = adjmatirx[origin][target].time;
                min_route.PS = tourist_spot[target].PS;
                min_route.route = {target};
                dfs(origin, {{}, 0, 0, 0}, {}, limit - 1);

                if (min_route.route.size() == 0) cout << "No such limited path to " << name << endl;
                else{
                    vector<int>::iterator it = min_route.route.begin();
                    cout << "Limited " << consideration << " : " << tourist_spot[origin].name << " -> ";
                    for(it ; it != min_route.route.end() - 1 ; it++)
                        cout << tourist_spot[*it].name << " -> ";
                    cout << tourist_spot[*it].name << endl;
                }
            }
        }
    }
    cout << "Stop receiving instructions" << endl;

    return 0;
}