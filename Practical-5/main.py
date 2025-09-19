def split_input(input_str):
    """Helper function to split comma-separated input into list of tokens"""
    return [token.strip() for token in input_str.split(",") if token.strip()]


def calculate_precision_recall(A, R):
    setA = set(A)
    setR = set(R)

    Ra = setA.intersection(setR)

    precision = len(Ra) / len(setA) if len(setA) > 0 else 0
    recall = len(Ra) / len(setR) if len(setR) > 0 else 0

    return len(Ra), precision, recall


def calculate_f_measure(precision, recall):
    return (2 * precision * recall / (precision + recall)) if (precision + recall) > 0 else 0


def calculate_e_measure(precision, recall, beta):
    if precision == 0 and recall == 0:
        return 0
    return ((1 + beta ** 2) * precision * recall) / ((beta ** 2 * precision) + recall) if ((beta ** 2 * precision) + recall) > 0 else 0


def generate_table(retrieved_docs, relevant_docs):
    print(f"{'Documents':<60} {'|Ra|':<5} {'|A|':<5} {'Precision':<10} {'Recall':<10}")
    print("-" * 95)

    results = []

    for i in range(1, len(retrieved_docs) + 1):
        current_set = retrieved_docs[:i]
        Ra_size, precision, recall = calculate_precision_recall(current_set, relevant_docs)

        results.append((precision, recall))

        docs_str = ", ".join(current_set)
        print(f"{docs_str:<60} {Ra_size:<5} {len(current_set):<5} {precision*100:<10.2f} {recall*100:<10.2f}")

    return results


def main():
    # Input retrieved documents
    retrieved_input = input("Enter the retrieved documents (comma-separated): ")
    retrieved_docs = split_input(retrieved_input)

    # Input relevant documents
    relevant_input = input("Enter the relevant documents (comma-separated): ")
    relevant_docs = split_input(relevant_input)

    # Generate table
    results = generate_table(retrieved_docs, relevant_docs)

    # Choose index j
    j = int(input(f"\nEnter value of j (0 - {len(results)-1}) to find F(j) and E(j): "))

    precision, recall = results[j]

    f1 = calculate_f_measure(precision, recall)

    e_greater = calculate_e_measure(precision, recall, 2)
    e_equal = calculate_e_measure(precision, recall, 1)
    e_less = calculate_e_measure(precision, recall, 0.5)

    print("\n---------------------------------")
    print(f"| Harmonic mean (F1) is: | {f1:.2f} |")
    print("---------------------------------\n")

    print("        ----------------------------")
    print("        |          E-Value          |")
    print("        ----------------------------")
    print("        |   b>1   |   b=1   |  b<1  |")
    print("        ----------------------------")
    print(f"        |  {e_greater:.2f}  |  {e_equal:.2f}  |  {e_less:.2f} |")
    print("        ----------------------------")


if __name__ == "__main__":
    main()
