"""
SimpleFOC Commander - Command-line interface for motor control
==============================================================

A CircuitPython implementation of the SimpleFOC Commander protocol for
interactive motor control and parameter tuning via serial terminal.

Compatible with SimpleFOC Arduino Commander syntax.

Author: CircuitPython SimpleFOC Project
License: MIT

Usage:
    from simplefoc_commander import Commander
    
    pid = simplefoc.PIDController(1.0, 0.5, 0.01, 100.0, 12.0)
    commander = Commander(pid=pid)
    
    while True:
        commander.run()  # Process commands non-blocking
        # ... motor control loop ...
"""

import supervisor
import sys

class Commander:
    """SimpleFOC-compatible command interface for CircuitPython
    
    Provides interactive serial terminal for:
    - PID parameter tuning (P, I, D, R, L)
    - LowPassFilter configuration (F)
    - Encoder monitoring
    - Target setting
    - Custom command callbacks
    
    Example:
        >>> from simplefoc_commander import Commander
        >>> import simplefoc
        >>> 
        >>> pid = simplefoc.PIDController(1.0, 0.5, 0.01, 100.0, 12.0)
        >>> commander = Commander(pid=pid, verbose=True)
        >>> 
        >>> while True:
        ...     commander.run()  # Non-blocking command processing
        ...     # Motor control loop here
    """
    
    def __init__(self, pid=None, lpf=None, encoder=None, verbose=True, decimal_places=3):
        """Initialize Commander
        
        Args:
            pid: simplefoc.PIDController instance (optional)
            lpf: simplefoc.LowPassFilter instance (optional)
            encoder: simplefoc.Encoder instance (optional)
            verbose: Enable verbose output (default: True)
            decimal_places: Number of decimal places for printing (default: 3)
        """
        self.pid = pid
        self.lpf = lpf
        self.encoder = encoder
        self.verbose = verbose
        self.decimal_places = decimal_places
        
        # Target value storage
        self.target = 0.0
        
        # Custom command callbacks
        self._callbacks = {}
        self._callback_labels = {}
        
        # Input buffer for character-by-character reading (NON-BLOCKING!)
        self._input_buffer = ""
        
        if self.verbose:
            self._print_welcome()
    
    def _print_welcome(self):
        """Print welcome message with available commands"""
        print("\n" + "="*50)
        print("SimpleFOC Commander - CircuitPython")
        print("="*50)
        print("Available commands:")
        if self.pid:
            print("  P[val]  - PID Proportional gain")
            print("  I[val]  - PID Integral gain")
            print("  D[val]  - PID Derivative gain")
            print("  R[val]  - PID Output ramp")
            print("  L[val]  - PID Output limit")
        if self.lpf:
            print("  F[val]  - LowPass Filter time constant")
        if self.encoder:
            print("  E       - Encoder status")
        print("  T[val]  - Set target value")
        print("  ?       - Print status")
        print("  V       - Toggle verbose mode")
        print("  @       - Scan registered commands")
        
        if self._callbacks:
            print("\nCustom commands:")
            for cmd_id, label in self._callback_labels.items():
                print(f"  {cmd_id}       - {label}")
        
        print("="*50)
        print("Ready! Type command and press Enter.")
        print()
    
    def add(self, command_id, callback, label=None):
        """Register a custom command callback
        
        Args:
            command_id: Single character command identifier (e.g., 'M')
            callback: Function to call, signature: callback(value_string)
            label: Optional description for help text
        
        Example:
            >>> def motor_enable(value):
            ...     if value == '1':
            ...         motor.enable()
            ...     elif value == '0':
            ...         motor.disable()
            >>> 
            >>> commander.add('M', motor_enable, "Motor enable/disable")
        """
        if len(command_id) != 1:
            raise ValueError("Command ID must be a single character")
        
        self._callbacks[command_id] = callback
        if label:
            self._callback_labels[command_id] = label
    
    def run(self):
        """Process incoming serial commands (NON-BLOCKING!)
        
        Reads character-by-character from serial, building up command buffer.
        Processes command when Enter is pressed.
        
        IMPORTANT: This is non-blocking! The loop continues running even
        while the user is typing a command.
        
        Example:
            >>> while True:
            ...     commander.run()  # Process commands (doesn't block!)
            ...     # Motor control code runs continuously
            ...     time.sleep(0.001)
        """
        # Check if serial data available
        if not supervisor.runtime.serial_bytes_available:
            return
        
        # Read one character at a time (NON-BLOCKING!)
        try:
            char = sys.stdin.read(1)
            
            if not char:
                return
            
            # Handle different control characters
            if char == '\n' or char == '\r':
                # End of line - process the command
                if self._input_buffer:
                    self.process_command(self._input_buffer)
                    self._input_buffer = ""
                    
            elif char == '\x03':  # Ctrl+C
                raise KeyboardInterrupt
                
            elif char == '\x08' or char == '\x7f':  # Backspace or Delete
                if self._input_buffer:
                    self._input_buffer = self._input_buffer[:-1]
                    
            elif ord(char) >= 32 and ord(char) < 127:  # Printable characters
                # Add character to buffer
                self._input_buffer += char
                
        except Exception as e:
            # Handle any input errors gracefully
            pass
    
    def process_command(self, cmd):
        """Process a complete command string
        
        Args:
            cmd: Command string (e.g., "P1.5", "?", "T100")
        
        This method can be called directly if you're reading commands
        from a source other than the serial terminal.
        
        Example:
            >>> commander.process_command("P1.5")  # Set P gain to 1.5
            >>> commander.process_command("?")      # Print status
        """
        if not cmd:
            return
        
        # Get command ID (first character)
        cmd_id = cmd[0].upper()
        
        # Get value (rest of string)
        value_str = cmd[1:].strip() if len(cmd) > 1 else ""
        
        # Custom callbacks take priority over all built-ins.
        # This allows scripts to redefine T, V, R, L, etc. for their own use.
        if cmd_id in self._callbacks:
            try:
                self._callbacks[cmd_id](value_str)
            except Exception as e:
                print(f"Error in custom command '{cmd_id}': {e}")

        # Built-in commands (only reached if no custom callback registered)
        elif cmd_id == 'P' and self.pid:
            self._handle_pid_P(value_str)
        elif cmd_id == 'I' and self.pid:
            self._handle_pid_I(value_str)
        elif cmd_id == 'D' and self.pid:
            self._handle_pid_D(value_str)
        elif cmd_id == 'R' and self.pid:
            self._handle_pid_ramp(value_str)
        elif cmd_id == 'L' and self.pid:
            self._handle_pid_limit(value_str)
        elif cmd_id == 'F' and self.lpf:
            self._handle_lpf(value_str)
        elif cmd_id == 'E' and self.encoder:
            self._handle_encoder(value_str)
        elif cmd_id == 'T':
            self._handle_target(value_str)
        elif cmd_id == '?':
            self._print_status()
        elif cmd_id == 'V':
            self._toggle_verbose()
        elif cmd_id == '@':
            self._scan_commands()
        else:
            print(f"Unknown command: {cmd_id}")
    
    # PID command handlers
    def _handle_pid_P(self, value_str):
        """Handle P (Proportional gain) command"""
        if value_str:
            # Set value
            try:
                value = float(value_str)
                self.pid.P = value
                if self.verbose:
                    print(f"P gain: {self._format_number(value)}")
            except ValueError:
                print("Error: Invalid number")
        else:
            # Get value
            print(f"P: {self._format_number(self.pid.P)}")
    
    def _handle_pid_I(self, value_str):
        """Handle I (Integral gain) command"""
        if value_str:
            try:
                value = float(value_str)
                self.pid.I = value
                if self.verbose:
                    print(f"I gain: {self._format_number(value)}")
            except ValueError:
                print("Error: Invalid number")
        else:
            print(f"I: {self._format_number(self.pid.I)}")
    
    def _handle_pid_D(self, value_str):
        """Handle D (Derivative gain) command"""
        if value_str:
            try:
                value = float(value_str)
                self.pid.D = value
                if self.verbose:
                    print(f"D gain: {self._format_number(value)}")
            except ValueError:
                print("Error: Invalid number")
        else:
            print(f"D: {self._format_number(self.pid.D)}")
    
    def _handle_pid_ramp(self, value_str):
        """Handle R (Output ramp) command"""
        if value_str:
            try:
                value = float(value_str)
                self.pid.output_ramp = value
                if self.verbose:
                    print(f"Output ramp: {self._format_number(value)}")
            except ValueError:
                print("Error: Invalid number")
        else:
            print(f"Ramp: {self._format_number(self.pid.output_ramp)}")
    
    def _handle_pid_limit(self, value_str):
        """Handle L (Output limit) command"""
        if value_str:
            try:
                value = float(value_str)
                self.pid.limit = value
                if self.verbose:
                    print(f"Output limit: {self._format_number(value)}")
            except ValueError:
                print("Error: Invalid number")
        else:
            print(f"Limit: {self._format_number(self.pid.limit)}")
    
    # LowPassFilter command handler
    def _handle_lpf(self, value_str):
        """Handle F (Filter time constant) command"""
        if value_str:
            try:
                value = float(value_str)
                self.lpf.time_constant = value
                if self.verbose:
                    print(f"Filter Tf: {self._format_number(value)} s")
            except ValueError:
                print("Error: Invalid number")
        else:
            print(f"Tf: {self._format_number(self.lpf.time_constant)}")
    
    # Encoder command handler
    def _handle_encoder(self, value_str):
        """Handle E (Encoder status) command"""
        angle = self.encoder.get_angle()
        velocity = self.encoder.get_velocity()
        position = self.encoder.position
        
        print(f"Encoder:")
        print(f"  Position: {position} counts")
        print(f"  Angle:    {self._format_number(angle)} rad")
        print(f"  Velocity: {self._format_number(velocity)} rad/s")
    
    # Target command handler
    def _handle_target(self, value_str):
        """Handle T (Target) command"""
        if value_str:
            try:
                value = float(value_str)
                self.target = value
                if self.verbose:
                    print(f"Target: {self._format_number(value)}")
            except ValueError:
                print("Error: Invalid number")
        else:
            print(f"Target: {self._format_number(self.target)}")
    
    # Utility commands
    def _toggle_verbose(self):
        """Toggle verbose mode"""
        self.verbose = not self.verbose
        print(f"Verbose: {'ON' if self.verbose else 'OFF'}")
    
    def _scan_commands(self):
        """Print all registered commands"""
        print("\n" + "="*50)
        print("Registered Commands:")
        print("="*50)
        
        if self.pid:
            print("PID Controller:")
            print("  P - Proportional gain")
            print("  I - Integral gain")
            print("  D - Derivative gain")
            print("  R - Output ramp")
            print("  L - Output limit")
        
        if self.lpf:
            print("\nLowPass Filter:")
            print("  F - Time constant (Tf)")
        
        if self.encoder:
            print("\nEncoder:")
            print("  E - Status and readings")
        
        print("\nGeneral:")
        print("  T - Target value")
        print("  ? - Print status")
        print("  V - Toggle verbose mode")
        print("  @ - Scan commands (this help)")
        
        if self._callbacks:
            print("\nCustom Commands:")
            for cmd_id, label in self._callback_labels.items():
                print(f"  {cmd_id} - {label}")
        
        print("="*50 + "\n")
    
    def _print_status(self):
        """Print current status of all components"""
        print("\n" + "="*50)
        print("System Status:")
        print("="*50)
        
        print(f"Target: {self._format_number(self.target)}")
        
        if self.pid:
            print("\nPID Controller:")
            print(f"  P:     {self._format_number(self.pid.P)}")
            print(f"  I:     {self._format_number(self.pid.I)}")
            print(f"  D:     {self._format_number(self.pid.D)}")
            print(f"  Ramp:  {self._format_number(self.pid.output_ramp)}")
            print(f"  Limit: {self._format_number(self.pid.limit)}")
        
        if self.lpf:
            print("\nLowPass Filter:")
            print(f"  Tf: {self._format_number(self.lpf.time_constant)} s")
        
        if self.encoder:
            angle = self.encoder.get_angle()
            velocity = self.encoder.get_velocity()
            position = self.encoder.position
            
            print("\nEncoder:")
            print(f"  Position: {position} counts")
            print(f"  Angle:    {self._format_number(angle)} rad")
            print(f"  Velocity: {self._format_number(velocity)} rad/s")
        
        print("="*50 + "\n")
    
    def _format_number(self, value):
        """Format number with configured decimal places"""
        return f"{value:.{self.decimal_places}f}"


# Convenience function for quick testing
def demo():
    """Run a Commander demo (for testing without hardware)"""
    print("SimpleFOC Commander Demo")
    print("This demo shows command parsing without hardware")
    print()
    
    class MockPID:
        def __init__(self):
            self.P = 1.0
            self.I = 0.5
            self.D = 0.01
            self.output_ramp = 100.0
            self.limit = 12.0
    
    mock_pid = MockPID()
    commander = Commander(pid=mock_pid, verbose=True)
    
    print("\nTry these commands:")
    print("  P1.5  - Set P gain to 1.5")
    print("  ?     - Print status")
    print("  V     - Toggle verbose")
    print()
    
    # Simulate some commands
    test_commands = ["P2.0", "I1.0", "D0.05", "?"]
    
    for cmd in test_commands:
        print(f"\n> {cmd}")
        commander.process_command(cmd)


if __name__ == "__main__":
    demo()
