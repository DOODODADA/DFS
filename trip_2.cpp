#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <set>
using namespace std;

struct vertex{
    string spot_name;
    int pre_scor;
    bool open_close;
};

struct edge{
    int time;
    float flow;
};

struct route{
    int time, pre_scor;
    float flow;
    vector<int> path;
    void init_route(int s, int d);
    void add_edge(int s, int d);
};

edge matrix[101][101];
vertex tourist_spots[100];
route ans_route{0, 0, 0, {}};
map<string, int> name_to_index;
int source, destination, target, num;
bool time_flow;


void route::init_route(int s, int d){
        time = matrix[s][d].time;
        flow = matrix[s][d].flow;
        pre_scor = tourist_spots[d].pre_scor;
        path = {d};
    }
void route::add_edge(int s, int d){
        flow += matrix[s][d].flow;
        time += matrix[s][d].time;
        pre_scor += tourist_spots[d].pre_scor;
        path.push_back(d);
    }

bool operator>(const route& lhs, const route& rhs){
    if (!time_flow)
    {
        if(lhs.flow == rhs.flow)
        {
            if(lhs.pre_scor == rhs.pre_scor)
            {
                for (int i = 0 ; i < min(lhs.path.size(), rhs.path.size()) ; i++)
                    if (lhs.path[i] != rhs.path[i]) return lhs.path[i] > rhs.path[i];
            }
            return lhs.pre_scor < rhs.pre_scor;
        }
        return lhs.flow > rhs.flow;
    }
    else{
        if(lhs.time == rhs.time)
        {
            if(lhs.pre_scor == rhs.pre_scor)
            {
                for (int i = 0 ; i < min(lhs.path.size(), rhs.path.size()) ; i++)
                    if (lhs.path[i] != rhs.path[i]) return lhs.path[i] > rhs.path[i];
            }
            return lhs.pre_scor < rhs.pre_scor;
        }
        return lhs.time > rhs.time;
    }
}

void print_route(route r, string tp, string consideration){
    cout << tp << " " << consideration << " : " << tourist_spots[source].spot_name;
    for (int i = 0 ; i < r.path.size() ; i++)
        cout << " -> " <<tourist_spots[r.path[i]].spot_name;
    cout << endl;
}

void dfs(int id, route rt, set<int> prev, int limit){
    if(id == destination)
    {
        if(ans_route > rt)
            ans_route = rt;
    }
    else if(limit == 0) return;
    else
    {
        for(int i=0 ; i<num ; i++)
        {
            if(matrix[id][i].time != 0 && tourist_spots[i].open_close)
            {
                if(!prev.count(i))
                {
                    route new_route = rt;
                    new_route.add_edge(id, i);
                    if(ans_route > new_route)
                    {
                        prev.insert(i);
                        dfs(i, new_route, prev, limit-1);
                        prev.erase(i);
                    }
                }
            }
        }
    }
}

int main(){
    cin >> num;
    for(int i=0 ; i<num ; i++)
    {
        string name;
        int id, score;
        cin >> id >> name >> score;
        name_to_index.insert(pair<string, int>(name, id));
        tourist_spots[id] = {name, score, true};
    }
    for(int i=0 ; i<num ;i++)
        for(int j=0 ; j<num ;j++)
            cin >> matrix[i][j].time;
    for(int i=0 ; i<num ;i++)
        for(int j=0 ; j<num ;j++)
            cin >> matrix[i][j].flow;
    for(int i=0 ; i<num ;i++)
        for(int j=0 ; j<num ;j++)
            matrix[i][j].flow = -log2(matrix[i][j].flow);
    
    string user_input;
    cin >> user_input;
    while(user_input != "END_OF_INSTRUCTION"){
        if(user_input == "SET_ORIGIN")
        {
            string name;
            cin >> name;
            if(tourist_spots[name_to_index[name]].open_close)
            {
                cout << name << " is the new origin" << endl;
                source = name_to_index[name];
            }
            else
                cout << "Fail to reset the origin" << endl;
        }
        else if(user_input == "MARK_STATE")
        {
            string state, name, lit_state;
            char c;
            bool o_c;
            cin >> state;
            o_c = (state == "OPEN")?true:false;
            lit_state = (state == "OPEN")?"open":"false";
            c = getchar();
            while(c != '\n'){
                cin >> name;
                int id = name_to_index[name];
                if(tourist_spots[id].open_close != o_c && (source != id || o_c != false))
                    tourist_spots[id].open_close = o_c;
                else cout << "Fail to " << lit_state << " " << name << endl;
                c = getchar();
            }
            
        }
        else if (user_input == "OPTIMAL_PATH")
        {
            string name, con;
            cin >> name >> con;
            destination = name_to_index[name];
            if(tourist_spots[destination].open_close == false)
                cout << "No such optimal path to " << name << endl;
            else if (source == target){
                cout << "Optimal " << con << " : " << name << endl;
            }
            else{
                time_flow = (con == "TIME")?true:false;
                ans_route.init_route(source, destination);
                dfs(source, {0,0,0,{}}, {}, 200);
                print_route(ans_route, "Optimal", con);
            } 
        }
        else if (user_input == "LIMITED_PATH")
        {
            string name, con;
            int limit;
            cin >> name >> con >> limit;
            destination = name_to_index[name];
            if(tourist_spots[destination].open_close == false)
                cout << "No such limited path to " << name << endl;
            else if (source == target){
                cout << "Limited " << con << " : " << name << endl;
            }
            else{
                time_flow = (con == "TIME")?true:false;
                ans_route.init_route(source, destination);
                dfs(source, {0,0,0,{}}, {}, limit-1);
                print_route(ans_route, "Limited", con);
            } 
        }
        cin >> user_input;
    }
    cout << "Stop receiving instructions" << endl;
    

}
