import subprocess
from pathlib import Path

BINARY_NAME = "Interpreter.exe"


def find_binary():
    project_folder = Path(__file__).resolve().parent

    possible_paths = [
        project_folder / "x64" / "Debug" / BINARY_NAME,
        project_folder / "x64" / "Release" / BINARY_NAME,
        project_folder / "Debug" / BINARY_NAME,
        project_folder / "Release" / BINARY_NAME,
        project_folder / BINARY_NAME
    ]

    for path in possible_paths:
        if path.exists():
            return path

    for path in project_folder.rglob(BINARY_NAME):
        if ".vs" not in path.parts:
            return path

    raise FileNotFoundError(
        f"Cannot find {BINARY_NAME}. Build the C++ project first."
    )


def run_interpreter(input_data):
    binary_path = find_binary()

    process = subprocess.Popen(
        [str(binary_path)],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    stdout, stderr = process.communicate(input_data)

    output_lines = []

    for line in stdout.splitlines():
        line = line.strip()

        if not line:
            continue

        if line == "Interpreter. Enter an expression:":
            continue

        output_lines.append(line)

    return output_lines, stderr.strip()


def run_tests():
    tests = [
        {
            "name": "Arithmetic priority",
            "input": "5 + 3 * 2\n",
            "expected": ["11"]
        },
        {
            "name": "Parentheses",
            "input": "(5 + 3) * 2\n",
            "expected": ["16"]
        },
        {
            "name": "Unary minus",
            "input": "-5 + 8\n",
            "expected": ["3"]
        },
        {
            "name": "Division",
            "input": "10 / 4\n",
            "expected": ["2.5"]
        },
        {
            "name": "Built-in pow",
            "input": "pow(2, 4)\n",
            "expected": ["16"]
        },
        {
            "name": "Built-in abs",
            "input": "abs(-7)\n",
            "expected": ["7"]
        },
        {
            "name": "Built-in max",
            "input": "max(5, 2)\n",
            "expected": ["5"]
        },
        {
            "name": "Built-in min",
            "input": "min(3, 4)\n",
            "expected": ["3"]
        },
        {
            "name": "Nested expressions",
            "input": "max(min(3 * 2, 4), pow(2, 3))\n",
            "expected": ["8"]
        },
        {
            "name": "Immutable variable",
            "input": "var a = 10\na + 3\n",
            "expected": ["13"]
        },
        {
            "name": "Variable with built-in function",
            "input": "var result = pow(2, 4)\nresult * 2\n",
            "expected": ["32"]
        },
        {
            "name": "Variable cannot be overwritten",
            "input": "var test = 5\nvar test = 20\ntest\n",
            "expected": [
                "Error: Variable 'test' is already defined",
                "5"
            ]
        },
        {
            "name": "User-defined function",
            "input": (
                "def myfunc(a, b) { min(a, b) + max(a, b) }\n"
                "myfunc(3, 4)\n"
            ),
            "expected": ["7"]
        },
        {
            "name": "Square function",
            "input": (
                "def square(x) { x * x }\n"
                "square(5)\n"
            ),
            "expected": ["25"]
        },
        {
            "name": "Function uses global variable",
            "input": (
                "var value = 10\n"
                "def addValue(x) { x + value }\n"
                "addValue(5)\n"
            ),
            "expected": ["15"]
        },
        {
            "name": "Wrong user function argument count",
            "input": (
                "def add(a, b) { a + b }\n"
                "add(5)\n"
            ),
            "expected": [
                "Error: Function add expects 2 arguments"
            ]
        },
        {
            "name": "Division by zero",
            "input": "10 / 0\n",
            "expected": [
                "Error: Division by zero"
            ]
        },
        {
            "name": "Unknown variable",
            "input": "unknown + 3\n",
            "expected": [
                "Error: Unknown variable: unknown"
            ]
        },
        {
            "name": "Wrong built-in function argument count",
            "input": "max(5)\n",
            "expected": [
                "Error: Function max expects 2 arguments"
            ]
        },
        {
            "name": "Unknown function",
            "input": "unknownFunction(5)\n",
            "expected": [
                "Error: Unknown function: unknownFunction"
            ]
        }
    ]

    passed = 0
    failed = 0

    for test in tests:
        try:
            output, error = run_interpreter(test["input"])
        except FileNotFoundError as error:
            print(error)
            return

        if error:
            print(f"[FAILED] {test['name']}")
            print(f"Error output: {error}")
            failed += 1
            continue

        if output == test["expected"]:
            print(f"[PASSED] {test['name']}")
            passed += 1
        else:
            print(f"[FAILED] {test['name']}")
            print(f"Expected: {test['expected']}")
            print(f"Received: {output}")
            failed += 1

    print()
    print("Test results")
    print(f"Passed: {passed}")
    print(f"Failed: {failed}")
    print(f"Total: {len(tests)}")


if __name__ == "__main__":
    run_tests()