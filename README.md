
# rys

`rys` is a lightweight CLI tool designed to automate the process of building Debian (`.deb`) packages from your project. It parses common project configuration files (`zyn.toml`, `CMakeLists.txt`) to extract package metadata, and packages your executable into a standard Debian package structure ready for installation.

---

## Features

- Automatically extracts package information from `zyn.toml` and `CMakeLists.txt`
- Supports different application types (console, GUI, service, library, script)
- Creates a properly structured Debian package directory tree
- Generates the Debian control file with customizable metadata
- Builds `.deb` package using the `dpkg-deb` tool
- Provides user prompts to fill in missing metadata interactively

---

## Installation

Install `rys` using .deb package:

```sh
url_deb_package: https://github.com/zyntraxis/rys/releases/tag/v1.0.0
```

Make sure you have installed rys_1.0.0.deb

```sh
sudo dpkg -i rys_1.0.0.deb
```

---

## Usage

Run the tool with the `build` command to create a `.deb` package from the current directory:

```sh
rys build
```

The tool will:

1. Detect and parse `zyn.toml` and/or `CMakeLists.txt` to gather package metadata.
2. Prompt for any missing metadata fields.
3. Prepare a Debian package directory structure in `./build_package/`.
4. Copy your executable to the appropriate directory inside the package.
5. Generate the Debian control file with metadata.
6. Build the `.deb` package using `dpkg-deb`.

---

## Configuration Files Supported

- `zyn.toml` — Extracts `name` and `version` from the project configuration.
- `CMakeLists.txt` — Parses the `project()` command to read the package name and version.

If neither file is present or metadata is incomplete, `rys` will prompt you to manually enter the required information.

---

## PackageInfo Structure

The package metadata managed by `rys` includes:

| Field             | Description                                                | Example                       |
| ----------------- | ---------------------------------------------------------- | ----------------------------- |
| `package_name`    | Name of the package                                        | `myapp`                       |
| `executable_file` | Path to the executable to package                          | `path_to_exec_file`           |
| `version`         | Package version                                            | `1.0.0`                       |
| `description`     | Short description of the package                           | `A sample CLI tool.`          |
| `maintainer`      | Package maintainer name and email                          | `John Doe <john@example.com>` |
| `arch`            | Target architecture (`amd64`, `arm`, `all`)                | `amd64`                       |
| `app_type`        | Type of app (`console`, `gui`, `service`, `lib`, `script`) | `console`                     |

---

## Example

Suppose your project directory contains:

- `zyn.toml` with:
  ```toml
  name = "myapp"
  version = "1.2.3"
  ```

- Executable located at `.zyn/build/myapp`

Running:

```sh
rys build
```

Will produce a Debian package in the current directory, ready for installation:

```sh
sudo dpkg -i myapp_1.2.3_amd64.deb
```

---

## Limitations and Notes

- The tool currently supports only Debian packaging (`.deb`).
- The executable file path must be correctly specified.
- `dpkg-deb` must be installed on your system.
- Basic error handling is implemented; further robustness improvements may be needed for complex projects.

---

## License

Zyntraxis License

---

## Author

Zyntraxis — [zyntraxis@gmail.com]

---

## Contribution

Contributions are welcome! Feel free to open issues or pull requests for bug fixes or feature suggestions.

---

## Contact

For questions or support, please contact zyntraxis@gmail.com