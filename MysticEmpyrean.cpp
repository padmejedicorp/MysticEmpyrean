#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <qiskit_cpp/qiskit_cpp.hpp>

int main() {
    // Create a quantum circuit with 3 qubits and 3 classical bits
    Qiskit::QuantumCircuit circuit(3, 3);

    // Create a random initial state for the qubit to be teleported
    std::vector<std::complex<double>> initial_state = {0.3, 0.6};  // Example state vector
    circuit.initialize(initial_state, 0);

    // Create entangled Bell pair (qubits 1 and 2)
    circuit.h(1);
    circuit.cx(1, 2);

    // Apply a CNOT gate and Hadamard gate to the qubit to be teleported (qubit 0)
    circuit.cx(0, 1);
    circuit.h(0);

    

    // Apply conditional gates based on measurement outcomes
    circuit.cx(1, 2);
    circuit.cz(0, 2);

    // Measure the result and store it in a classical bit (qubit 2)
    circuit.measure(2, 2);

    // Simulate the quantum circuit using a local simulator
    Qiskit::AerBackend simulator("qasm_simulator");
    Qiskit::TranspileConfig transpile_config;
    transpile_config.backend(simulator);
    Qiskit::Qobj qobj = circuit.assemble(transpile_config);
    Qiskit::IBMQJob job = simulator.run(qobj);

    // Wait for the job to finish
    job.waitForFinalState();

    // Get the result
    Qiskit::Result result = job.getResult();

    // Display the measurement results
    Qiskit::Counts counts = result.getCounts();
    std::cout << "Measurement results:" << std::endl;
    std::cout << counts << std::endl;

    // Visualize the quantum state on the Bloch sphere for the teleported qubit (qubit 2)
    Qiskit::BlochSphere bloch_sphere(result.getStatevector(2));
    bloch_sphere.plot("Bloch Sphere");

    // Visualize the measurement results as a histogram
    Qiskit::Histogram histogram(counts);
    histogram.plot("Measurement Histogram");

    return 0;
}
