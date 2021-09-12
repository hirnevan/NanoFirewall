/*
 * WebManager.h
 *
 *  Created on: Nov 10, 2020
 *      Author: 
 */

#ifndef WEBMANAGER_H_
#define WEBMANAGER_H_

#include <restbed>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

class WebManager {
 public:
  WebManager();
  ~WebManager();
  void startWebAPI();

 private:
  void ruleGetHandler(const std::shared_ptr<restbed::Session> session);
  void rulePatchHandler(const std::shared_ptr<restbed::Session> session);
  void settingsGetHandler(const std::shared_ptr<restbed::Session> session);
  void settingsPatchHandler(const std::shared_ptr<restbed::Session> session);
  void loginHandler(const std::shared_ptr<restbed::Session> session);
  void authenticateRequest(
      const std::shared_ptr<restbed::Session> session,
      const std::function<void(const std::shared_ptr<restbed::Session>)>
          &callback);
  void statusHandler(const std::shared_ptr<restbed::Session> session);
  void filterListPostHandler(const std::shared_ptr<restbed::Session> session);
  void nicknamePatchHandler(const std::shared_ptr<restbed::Session> session);
  void nicknameGetHandler(const std::shared_ptr<restbed::Session> session);
  void statsGetHandler(const std::shared_ptr<restbed::Session> session);
  static inline void addEachFilterList(std::ifstream &file, std::string &LINE);
  static inline void removeEachFilterList(std::ifstream &file,
                                          std::string &LINE);
  restbed::Service service;
};

#endif /* WEBMANAGER_H_ */
