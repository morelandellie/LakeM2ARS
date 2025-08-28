# LakeM<sup>2</sup>ARS

Files provided in this repository are examples from a previous run. Users should download and edit the files accordingly to run the LakeM<sup>2</sup>ARS model to their specifications. 

## Model Description
LakeM<sup>2</sup>ARS is adapted from the PRYSM v2.0 Lake Water Energy Balance Model, a one-dimensional lake thermal and hydrological model (Dee et al., 2018; Hostetler et al., 1990; Morrill et al., 2001; Patterson et al., 1988; Small et al., 1999). PRYSM was built to model relationships between climate, lake properties, and paleoclimate proxy data, and LakeM2ARS is the model to be used in a Martian environment. The model has the ability to simulate lake energy and water balance. Detailed descriptions of the PRYSM model are documented in Dee et al. (2018) and the model can be downloaded from GitHub (https://github.com/sylvia-dee/PRYSM). Adaptations to PRYSM to create LakeM<sup>2</sup>ARS are documented in the manuscript.

### Input file

The lake model requires seven climate input variables including near-surface air temperature (K), near-surface relative humidity (%), downward shortwave radiation (W/m<sup>2</sup>), downward longwave radiation (W/m<sup>2</sup>), near-surface wind speed (m/s), surface pressure (Pa), precipitation (mm), and basin runoff (mm/day). In the provided example, the model is configured to Gale crater, Mars, and the input variables are obtained from the Mars Weather Research and Forecasting Global Climate Model (MarsWRF GCM).

One climate variable input file is required. It contains all necessary meteorological variables to force the model. In the example, monthly mean climate values were used to drive the model simulations. 

The input file is in plain text format and can be named as **<mars_met_data.txt>**, or whatever you prefer. If users change the name of the .txt input file, they must ensure the change of file name is reflected in the .f90 code – see section “Model compilation and running” below. There is no header row in the file. Values in the file follows the structure as below:

Column 1. Year

Column 2. Day of year (accumulative)

Column 3. Air temperature at 5 meters (K or C, specify in the .inc file)

Column 4. Relative humidity at 5 meters (%)

Column 5. Wind speed at 5 meters (m/s)

Column 6. Surface incident shortwave radiation (W/m<sup>2</sup>)

Column 7. Downward longwave radiation (W/m<sup>2</sup>)

Column 8. Surface pressure (Pa)

Column 9. Precipitation (mm)

Column 10. Basin runoff (mm)

*Columns 1 to 8 are required. Columns 9 and 10 are only used when water balance is modeled.*

### Model parameters
Major model parameters are defined in the **<mars_lake.inc>** file, which contains lake specific parameters, simulation specific parameters and other fundamental physical and chemical parameters having fixed values. If users change the name of the .inc file, ensure the .f90 file is updated accordingly. 

In the section of lake specific parameters, we specify the Mars’ obliquity (degrees), the lake’s latitude, longitude, maximum lake depth (meters), the elevation of the basin bottom (meters), the area of the drainage basin when lake depth equals zero (hectares), the neutral drag coefficient (unitless), shortwave extinction coefficient (meters-1), the fraction of advected air in the air mass over the lake (ranges from 0 to 1), albedo of melting and non-melting snow, prescribed or initial lake depth (meters, typically represents mean lake depth), prescribed or initial lake salinity (parts per thousand), and initial lake temperature (°C).

In the section of simulation specific parameters, we specify the number of years for spinup, and turn on/off the water balance calculations, lake ice, and variable salinity. In the section of fundamental physical and chemical parameters, we specify Mars' specific heat capacity for dry air, specific gas constant for dry air, longwave emissivity, orbital degrees per day, and gravity.

### Output files

Two output files are generated from the model. One file is named as <lake_surf.dat>, which contains monthly mean values of the following variables:

Column 1. Day of year (accumulative)

Column 2. Lake surface temperature (°C, averaged over top 1 meter)

Column 3. Average mixing depth (m)

Column 4. Lake evaporation (mm/day)

Column 5. Latent heat flux (W/m<sup>2</sup>)

Column 6. Sensible heat flux (W/m<sup>2</sup>)

Column 7. Shortwave radiation upward (W/m<sup>2</sup>)

Column 8. Longwave radiation upward (W/m<sup>2</sup>)

Column 9. Ice height (m)

Column 10. Ice fraction (ranges from 0 to 1)

Column 11. Maximum mixed layer depth (m)

Column 12. Lake depth (m)

The other output file is named as **<lake_Tprof.dat>**, which contains the lake temperature profile from lake surface to the bottom for each meter depth. There are no header rows in either output file.

### Model compilation and running

To install LakeM<sup>2</sup>ARS v2.0, a user needs to have a working Fortran compiler. Gfortran compiler is recommended (https://directory.fsf.org/wiki/Gfortran). In the provided example, the model was coded in Fortran and compiled using gfortran. All model functions are coded in the .f90 file. Model configurations and parameters are defined in the mars_lake.inc file as described above.

Before compiling the model, a user needs to first adjust all necessary parameter values to the studied lake in the mars_lake.inc file. The mars_lake.inc file should be in the same path of the .f90 file. Moreover, the path for the climate input file should be defined in the mars_lake.inc file by changing the ‘datafile’ variable. Paths for the two output files can be defined at the ‘file_open’ subroutine in the .f90 code file.

Once mars_lake.inc is ready to use, a user can navigate to the folder containing:
1. Folder named "input", containiing only the **<mars_met_data.txt>** input file
2. Folder named "output", which starts empty and will be populated with the output files
3. The .f90 file
4. The .inc file

Compile the code in terminal (Mac or Linux) or CMD (Windows) by ***editing*** and using the following command, as from the provided example:
```
gfortran -o WB_C5 cold_w5.f90
```
Where *WB_C5* is a user-determined name of the model run, and *cold_w5.f90* is the name of the .90 code file. 

Once the executable file ‘lakerun’ is created, the model can be run by ***editing*** and using the command:
```
./WB_C5
```
Where *WB_C5* is a user-determined name of the model run.

Once the model run is complete, results can be found in the "output" folder.
