classDiagram
    class NodeTraverser {
    }
    class NodeVisitor {
        <<interface>>
    }
    class NodeVisitorAbstract {
        <<abstract>>
    }
    class FindingVisitor {
    }
    class NameResolver {
    }
    class Node {
    }

    NodeVisitor <|.. NodeVisitorAbstract
    NodeVisitorAbstract <|-- FindingVisitor
    NodeVisitorAbstract <|-- NameResolver
    NodeTraverser o-- NodeVisitor
    NodeTraverser --> Node
    NodeVisitor --> Node
