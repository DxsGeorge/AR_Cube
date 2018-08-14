#include "Input.hpp"

using namespace std;

string Input(string faceletStrings[6]) {
    unsigned int status;

	int a;
	/*
	string faceletStrings[6]
		=
	{
		"U:RWGGWRWWW", "D:YBGGYYBOW", "F:RRROBYRWW", "B:OOYOGROYY", "L:GWBGOBOYB", "R:GBYRRGOBB"
	};
	*/
    // Parse the input and initialize FaceletCube
    FaceletCube faceletCube;
    CubeParser cubeParser;
    if((status = cubeParser.parseFacelets(faceletStrings, faceletCube)) != CubeParser::VALID) {
        cout << cubeParser.ErrorText(status) << endl;
		string str = "error";
		return str;
    }
    
    // Validate the FaceletCube
    RubiksCube cube;
    if((status = faceletCube.Validate(cube)) != FaceletCube::VALID) {
        cout << faceletCube.ErrorText(status) << endl;
		string str = "error";
		return str;
    }
    
    // Cube is in a valid configuration at this point
    
    // Initialize tables and solve
	string solution;
    Solver solver;
    solver.InitializeTables();
    solution = solver.Solve(cube);
	cout << solution << endl;
	return solution;
	
}	
