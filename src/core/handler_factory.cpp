#include "handler_factory.hpp"
#include "event_arrived_handler.hpp"
#include "event_left_handler.hpp"
#include "event_seated_handler.hpp"
#include "event_waiting_handler.hpp"
#include <memory>

namespace core {

HandlerFactory::HandlerFactory() {
  handlers_[EventType::CLIENT_ARRIVED] =
      std::make_unique<ClientArrivedHandler>();
  handlers_[EventType::CLIENT_SEATED] = std::make_unique<ClientSeatedHandler>();
  handlers_[EventType::CLIENT_WAITING] =
      std::make_unique<ClientWaitingHandler>();
  handlers_[EventType::CLIENT_LEFT] = std::make_unique<ClientLeftHandler>();
  handlers_[EventType::CLIENT_SEATED_FROM_QUEUE] =
      std::make_unique<ClientSeatedHandler>();
}

EventHandler *HandlerFactory::get_handler(EventType type) const {
  const auto it = handlers_.find(type);
  return (it != handlers_.end()) ? it->second.get() : nullptr;
}

} // namespace core
