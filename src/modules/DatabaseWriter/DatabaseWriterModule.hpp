/**
 * @file
 * @brief Definition of database writer module
 * @copyright Copyright (c) 2020 CERN and the Allpix Squared authors.
 * This software is distributed under the terms of the MIT License, copied verbatim in the file "LICENSE.md".
 * In applying this license, CERN does not waive the privileges and immunities granted to it by virtue of its status as an
 * Intergovernmental Organization or submit itself to any jurisdiction.
 */

#include <atomic>
#include <fstream>
#include <map>
#include <string>

#include "core/config/Configuration.hpp"
#include "core/geometry/GeometryManager.hpp"
#include "core/messenger/Messenger.hpp"
#include "core/module/Module.hpp"

#include <pqxx/pqxx>

namespace allpix {
    /**
     * @ingroup Modules
     * @brief Module to write object data to PostgreSQL databases
     *
     * Listens to all objects dispatched in the framework and stores an ASCII representation of every object to file.
     */
    class DatabaseWriterModule : public BufferedModule {
    public:
        /**
         * @brief Constructor for this unique module
         * @param config Configuration object for this module as retrieved from the steering file
         * @param messenger Pointer to the messenger object to allow binding to messages on the bus
         * @param geo_mgr Pointer to the geometry manager, containing the detectors
         */
        DatabaseWriterModule(Configuration& config, Messenger* messenger, GeometryManager* geo_mgr);

        /**
         * @brief Receive a single message containing objects of arbitrary type
         * @param message Message dispatched in the framework
         * @param name Name of the message
         */
        bool filter(const std::shared_ptr<BaseMessage>& message, const std::string& name) const;

        /**
         * @brief Opens the database to write the objects to
         */
        void init() override;

        /**
         * @brief Writes the objects fetched to their specific tree, constructing trees on the fly for new objects.
         */
        void run(Event* event) override;

        /**
         * @brief Add the main configuration and the detector setup to the data file and write it, also write statistics
         * information.
         */
        void finalize() override;

    private:
        Messenger* messenger_;

        // Object names to include or exclude from writing
        std::set<std::string> include_;
        std::set<std::string> exclude_;

        // postgreSQL objects
        std::shared_ptr<pqxx::connection> conn_;
        std::shared_ptr<pqxx::nontransaction> W_;
        std::string host_;
        std::string port_;
        std::string database_name_;
        std::string user_;
        std::string password_;
        std::string run_id_;
        int run_nr_;

        // Statistical information about number of objects
        std::atomic<unsigned long> write_cnt_{};
        std::atomic<unsigned long> msg_cnt_{};
    };
} // namespace allpix
