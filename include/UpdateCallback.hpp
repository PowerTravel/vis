#ifndef UPDATE_CALLBACK_HPP  
#define UPDATE_CALLBACK_HPP

class Node;

/*	
 *	Class: 		UpdateCallback
 *	Purpose: 	BaseClass for all callback classes
 *	Misc:		All other callbacks inherit from this one
 */
class UpdateCallback{

	public:
		virtual void execute() = 0;	
	private:

};


#endif // UPDATE_CALLBACK_HPP

