classDiagram
    class IsEqual {
    }
    class Comparator {
        <<abstract>>
    }
    class ArrayComparator {
    }
    class NumericComparator {
    }
    direction LR
    IsEqual --> Comparator
    Comparator <|-- ArrayComparator
    Comparator <|-- NumericComparator
    end