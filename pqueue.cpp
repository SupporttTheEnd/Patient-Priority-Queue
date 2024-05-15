// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {
    m_heap = nullptr;
    m_size = 0;
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
}

PQueue::~PQueue() {
    clear(m_heap);
    m_heap = nullptr;
    m_size = 0;
}

void PQueue::clear() {
    clear(m_heap);
    m_heap = nullptr;
    m_size = 0;
}

void PQueue::clear(Node*& node){
    // check if the node is populated; if it is, call clear recursively on the left and right
    if (node) {
        clear(node->m_left);
        clear(node->m_right);
        delete node;
        node = nullptr;
    }
}

PQueue::PQueue(const PQueue& rhs) {
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_size = rhs.m_size;

    m_heap = copy(rhs.m_heap);
}

Node* PQueue::copy(const Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    Node* newNode = new Node(node->getPatient());

    newNode->m_left = copy(node->m_left);
    newNode->m_right = copy(node->m_right);

    return newNode;
}

PQueue& PQueue::operator=(const PQueue& rhs) {
     // check if lhs is the rhs
    if (this == &rhs) {
        return *this;
    }
    // clear out the tree
    clear(m_heap);

    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_size = rhs.m_size;
    
    // call the copy operation
    m_heap = copy(rhs.m_heap);
    return *this;
}

void PQueue::mergeWithQueue(PQueue& rhs) {
    // Check if priority functions and data structures match
    if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure) {
        throw domain_error("Invalid merge operation: Priority functions and or data structures mismatch.");
    }

    // Check if trying to merge with itself
    if (this == &rhs) {
        return; 
    }

    // Merge the heaps using recursive merge function
    m_heap = merge(m_heap, rhs.m_heap);
    m_size += rhs.m_size;

    // Clear the merged queue
    rhs.clear();
}

Node* PQueue::merge(Node* heap1, Node* heap2) {
    if (heap1 == nullptr) {
        return heap2;
    }
    if (heap2 == nullptr) {
        return heap1;
    }

    // Ensure min-heap or max-heap property based on the heap type
    if ((m_heapType == MINHEAP && m_priorFunc(heap1->getPatient()) > m_priorFunc(heap2->getPatient())) ||
        (m_heapType == MAXHEAP && m_priorFunc(heap1->getPatient()) < m_priorFunc(heap2->getPatient()))) {
        swap(heap1, heap2);
    }

    // Merge for leftist heap (maintain NPL)
    if (m_structure == LEFTIST) {
        heap1->m_right = merge(heap1->m_right, heap2);
        if (heap1->m_left == nullptr || heap1->m_left->getNPL() < heap1->m_right->getNPL()) {
            swap(heap1->m_left, heap1->m_right);
        }
        if (heap1->m_right != nullptr) {
            heap1->setNPL(heap1->m_right->getNPL() + 1);
        } 
        else {
            heap1->setNPL(0);
        }
    } 
    // Merge for skew heap
    else if (m_structure == SKEW) {
        swap(heap1->m_left, heap1->m_right);
        heap1->m_left = merge(heap2, heap1->m_left);
    }
    return heap1;
}

void PQueue::insertPatient(const Patient& input) {
    Node* newNode = new Node(input);

    m_heap = merge(m_heap, newNode);
    m_size++;
}

int PQueue::numPatients() const {
    return m_size;
}

prifn_t PQueue::getPriorityFn() const {
   return m_priorFunc;
}

Patient PQueue::getNextPatient() {
    if (m_heap == nullptr) {
        throw out_of_range("Queue is empty.");
    }

    // Extract the highest priority patient
    Patient nextPatient = m_heap->getPatient();

    Node* temp = m_heap;
    m_heap = merge(m_heap->m_left, m_heap->m_right);
    delete temp;

    // Decrement the size of the queue
    m_size--;

    return nextPatient;
}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    int index = 0; 
    Patient* patients = new Patient[m_size];
    collectPatients(m_heap, patients, index);
    
    clear();

    m_priorFunc = priFn;
    m_heapType = heapType; 

    for (int i = 0; i < index; i++) {
        insertPatient(patients[i]);
    }
    delete[] patients;
}

void PQueue::setStructure(STRUCTURE structure){
    int index = 0; 
    Patient* patients = new Patient[m_size];
    collectPatients(m_heap, patients, index);

    clear();

    m_structure = structure;

    for (int i = 0; i < index; ++i) {
        insertPatient(patients[i]);
    }

    delete[] patients;
}

STRUCTURE PQueue::getStructure() const {
  return m_structure; 
}

void PQueue::printPatientQueue() const {
    printPatientQueue(m_heap);
}

void PQueue::printPatientQueue(Node* node) const {
    if (node) {
        
        cout << "[" << m_priorFunc(node->getPatient()) << "]" << *node << endl;
        
        printPatientQueue(node->m_left);
        printPatientQueue(node->m_right);
    }
}

void PQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}

void PQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
    else
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout  << patient.getPatient() 
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}

void PQueue::swap(Node*& first, Node*& second) {
    Node* temp = first;
    first = second;
    second = temp;
}

void PQueue::collectPatients(Node* node, Patient* patients, int& index) {
    if (node) {
        collectPatients(node->m_left, patients, index);
        patients[index++] = node->m_patient;
        collectPatients(node->m_right, patients, index);
    }
}