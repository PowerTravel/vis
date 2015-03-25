#ifndef UPDATE_CALLBACK_HPP  
#define UPDATE_CALLBACK_HPP

#include <memory>

class Node;

#ifndef CALLBACK_PTR
#define CALLBACK_PTR
class UpdateCallback;
typedef std::shared_ptr<UpdateCallback> callback_ptr;
#endif // CALLBACK_PTR

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

