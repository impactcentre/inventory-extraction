/*
 * ParsingThread.hpp
 *
 *  Created on: 04.02.2010
 *      
 */

#ifndef PARSINGTHREAD_HPP_
#define PARSINGTHREAD_HPP_

#include <QThread>


namespace impact_tr4 {

class ParsingThread : public QThread
{
	Q_OBJECT
public:
	ParsingThread();
	~ParsingThread() {};

	virtual void run();
private:


}; // end of class ParsingThread.hpp

} // end of namespace impact_tr4


#endif /* PARSINGTHREAD_HPP_ */
