### Build UltraZed-EG-IOCC Hardware

#### Create Project

```
shell$ cd target/UltraZed-EG-IOCC/vivado-project/
shell$ vivado -mode batch -source create_project.tcl
```

#### Implementation

```
shell$ vivado -mode batch -source implementation.tcl
```

#### Export Hadware to project.sdk

```
shell$ vivado -mode batch -source export_hardware.tcl
```
