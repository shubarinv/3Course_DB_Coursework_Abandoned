//
// Created by vhund on 24.03.2021.
//

#ifndef DB_QT_COURSEWORK__FUNCTIONS_HPP_
#define DB_QT_COURSEWORK__FUNCTIONS_HPP_
#include <future>
namespace vh {
    template<typename T>
    bool checkIfAsyncTaskFinished(std::future<T> &asyncTask) {
        return asyncTask.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }
}// namespace vh
#endif//DB_QT_COURSEWORK__FUNCTIONS_HPP_
