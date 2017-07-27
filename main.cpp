#include "main.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <math.h>

using namespace std;
using namespace sf;

/* Structure for easy interaction with map */
struct Map{
	
public:
	Map() : n(0), m(0) {
		arr = new char[]; 
		rect = new RectangleShape[]; 
	}
	Map(int n, int m) : n(n), m(m) {
		arr = new char[n*m]; 
		rect = new RectangleShape[n*m]; 
	}
	~Map() {
		delete [] arr; 
		delete [] rect;
	}
	
	char & getValue(int i, int j){ return *(arr + i * m  + j); }
	RectangleShape & getRect(int i, int j){ return *(rect + i * m  + j); }

	RectangleShape * rect;
	char * arr;
	int const m;
	int const n;

};

/* Structure for easy interaction with Player */
struct Player{

public:
	Player() : x(0), y(0) {}
	Player(int x, int y) : x(x), y(y) {}
	~Player() {}
	int x;
	int y;
	bool hasRight( Map & map ) { 
		if( x+1 > map.n-1 ) return false;
		return map.getValue(  y, x+1 ) == '2'; 
	}
	bool hasLeft( Map & map ) { 
		if( x-1 < 1 ) return false;
		return map.getValue( y, x-1 ) == '2'; 
	}
	bool hasUp( Map & map ) { 
		if( y-1 < 1 ) return false;
		return map.getValue( y-1, x ) == '2'; 
	}
	bool hasDown( Map & map ) { 
		if( y+1 > map.m-1 ) return false;
		return map.getValue( y+1, x ) == '2'; 
	}

};

/* Algorithm of generation of labyrinth */
void genLab(Map & map, int i, int j){

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	if( j+2 < map.n ) if( map.getValue( i, j+2 ) == '0') right = true;
	if( i-2 > 0 ) if( map.getValue( i-2, j ) == '0') up = true;
	if( j-2 > 0 ) if( map.getValue( i, j-2 ) == '0') left = true;
	if( i+2 <  map.m ) if( map.getValue( i+2, j ) == '0') down = true;

	if(!left && !right && !up && !down) return;

	bool check = true;

	while(check){
	
		short s = 1 + rand() % 4;
		
		switch(s){
		case 1:{
			if(right){
				map.getValue( i, j+1 ) = '2';
				map.getValue( i, j+2 ) = '2';
				genLab( map, i, j+2 );
				genLab( map, i, j );
				check = false;
			}
			break;
		}
		case 2:{
			if(up){
				map.getValue( i-1, j ) = '2';
				map.getValue( i-2, j ) = '2';
				genLab( map, i-2, j );
				genLab( map, i, j );
				check = false;
			}
			break;
		}
		case 3:{
			if(left){
				map.getValue( i, j-1 ) = '2';
				map.getValue( i, j-2 ) = '2';
				genLab( map, i, j-2 );
				genLab( map, i, j );
				check = false;
			}
			break;
		}
		case 4:{
			if(down){
				map.getValue( i+1, j ) = '2';
				map.getValue( i+2, j ) = '2';
				genLab( map, i+2, j );
				genLab( map, i, j );
				check = false;
			}
			break;
		}
		}

	}
	
	return;
}

/* Addition some rooms n * m to the Map map */
void genRooms(Map & map, int n, int m){
	
	short int amount = 5 + rand() % 15;

	for( int i = 0; i < amount; i++ ){

		short int x = 1 + rand() % (map.n - 1);
		short int y = 1 + rand() % (map.m - 1);

		if( x + m < map.n && y + m < map.m ){
			for( int a = x; a < x + n; a++ ){
				for( int b = y; b < y + m; b++ ){
					map.getValue( a, b ) = '2';
				}
			}
		}
	}

	return;
}

int main()
{
	int a, b;
	a = 726;
	b = 726;
    RenderWindow window(VideoMode(a, b), "LabGen");

	srand( time(0) );

	Map map(121, 121);
	for( int i = 0; i < map.n; ++i )
		for( int j = 0; j < map.m; ++j )
				map.getValue( i, j ) = '0';
	for( int i = 0; i < map.n; i += 2 )
		for( int j = 0; j < map.m; ++j )
				map.getValue( i, j ) = '1';
	for( int i = 0; i < map.n; ++i )
		for( int j = 0; j < map.m; j += 2 )
				map.getValue( i, j ) = '1';

	genLab(map, 1, 1);

	Player player(1, 1);

	Vector2f rectSize;
	rectSize.x = a/map.n;
	rectSize.y = b/map.m;

    while ( window.isOpen() )
    {
        Event event;
        while ( window.pollEvent(event) )
        {
            if ( event.type == Event::Closed )
                window.close();
        }

		if( Keyboard::isKeyPressed( Keyboard::Left ) )
			if( player.hasLeft( map ) ) player.x -= 1;
		if( Keyboard::isKeyPressed( Keyboard::Right ) )
			if( player.hasRight( map ) ) player.x += 1;
		if( Keyboard::isKeyPressed( Keyboard::Up ) )
			if( player.hasUp( map ) ) player.y -= 1;
		if( Keyboard::isKeyPressed( Keyboard::Down ) )
			if( player.hasDown( map ) ) player.y += 1;

		for( int i = 0; i < map.n; ++i ){
			for( int j = 0; j < map.m; ++j ){

			Vector2f pos;
			pos.x = j * rectSize.x;
			pos.y = i * rectSize.y;

			map.getRect( i,j ).setSize( rectSize );
			map.getRect( i,j ).setPosition( pos );

			if( map.getValue( i, j ) == '0' ){
				map.getRect( i, j ).setFillColor( Color::White );
			}else{
				map.getRect( i, j ).setFillColor( Color::Black );
			}

			switch ( map.getValue( i, j ) ){
				case ('0') : {
					map.getRect( i, j ).setFillColor( Color::Yellow);
					break;
				}
				case ('1') : {
					Color grey;
					grey.r = 0x99;
					grey.g = 0x66;
					grey.b = 0x33;
					map.getRect( i, j ).setFillColor( grey );
					break;
			    }
				case ('2') : {
					map.getRect( i, j ).setFillColor( Color::Black );
					break;
				}
				default:	 {
					break;
				}
			}
			map.getRect( player.y, player.x ).setFillColor( Color::Red );
			map.getRect( map.n-2, map.m-2 ).setFillColor( Color::Blue );
		}
	}

        window.clear();
		for( int i = 0; i < map.n; ++i )
			for( int j = 0; j < map.m; ++j )
				window.draw( map.getRect( i, j ) );
        window.display();
    }

    return 0;
}