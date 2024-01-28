classDiagram
  class FooListener {
    <<Observer>>
  }
  class DispatcherInterface {
    <<Subject>>
  }
  class Dispatcher {
  }
  direction BT
  Dispatcher ..|> DispatcherInterface
  Dispatcher o-- FooListener
  end