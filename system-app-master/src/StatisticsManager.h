/*
 * StatisticsManager.h
 *
 *  Created on: Apr 1, 2021
 *      Author: 
 */

#ifndef STATISTICSMANAGER_H_
#define STATISTICSMANAGER_H_

class StatisticsManager {
 private:
  void update_day(int);

 public:
  StatisticsManager();
  ~StatisticsManager();
  void add_violation();
};

#endif /* STATISTICSMANAGER_H_ */
