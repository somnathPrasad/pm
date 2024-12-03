# **PM: Password Manager**

A simple command-line password manager written in C. This program allows users to securely store, retrieve, and manage passwords associated with specific keys locally.

## **Features**

- Add new passwords.
- List all saved passwords.
- Find a password by its key.
- Save passwords persistently in a file (`passwords.txt`).
- User-friendly commands with help instructions.

## **Getting Started**

### **Prerequisites**

- A C compiler (e.g., GCC or Clang).
- Basic understanding of the command line.

### **Setup Instructions**

1. **Clone the repository:**

   ```bash
   git clone https://github.com/<your-username>/<your-repo-name>.git
   cd <your-repo-name>
   ```

2. **Compile the program:**

   ```bash
   gcc -o pm pm.c
   ```

3. **Run the program:**
   ```bash
   ./pm
   ```

### **Usage**

#### **Commands**

| Command                | Description                                           | Example                         |
| ---------------------- | ----------------------------------------------------- | ------------------------------- |
| `new <key>:<password>` | Add a new password with the specified key.            | `new email:mysecurepassword123` |
| `list`                 | List all saved passwords.                             | `list`                          |
| `find <key>`           | Retrieve the password associated with a specific key. | `find email`                    |
| `help`                 | Display the list of available commands.               | `help`                          |

## **Example**

### **Adding a New Password**

```bash
./pm new email:mysecurepassword123
```

### **Listing All Saved Passwords**

```bash
./pm list
```

### **Finding a Password**

```bash
./pm find email
```

### **Displaying Help**

```bash
./pm help
```

## **Contributing**

Contributions are welcome! To contribute:

1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add a new feature"
   ```
4. Push to the branch:
   ```bash
   git push origin feature-name
   ```
5. Open a pull request.

## **Future Enhancements**

- Encrypt passwords for additional security.
- Add a command to delete passwords.
- Support for importing/exporting passwords.
