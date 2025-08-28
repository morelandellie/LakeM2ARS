! Version 1.0
! Declarations ********************************************************
      real dz,dt,readdt,basedep,zo,z_screen,delta,rhowat,rhoice,rhosnow
      real Le,Lei,fusion,surf,fracmin,fraclim,qwtau,cpw_ice,condw,beta
      real qhw_ave, qew_ave, luw_ave, sww_ave, qhw, qew, sww, luw
      real snocrit,kv,dm,pi,tsurfp,mixing,trace,depth_begin,salty_begin
      real trace_i,trace_a,d_frac,tempi_a,hice_a,hsnow_a,xlat,xlon,gmt
      real b_area,runout_sum,alb_slush,alb_snow,fsol1,fsol2,sical0
      real sical1
      real salty_a,eta,fraci_a,temp_a,d_fraca,snowcut,o18air,f,ti_a
      real deutair,dexch,alphak,lamisw,lamilw,ca,cb,c70,c71,c72,c73
      real lamssw,lamslw,afrac1,afrac2,condi,conds,cpair,emis
      real o18snow_a,deutsnow_a,psum_a,surfarea_a,hsnow_ave,temp_ave
      real o18pro_ave,deutpro_ave
      real tsurf_ave,fice_ave,evap_ave,hice_ave,o18_ave,deut_ave
      real oblq,cdrn,raddeg,dpd,grav,sigma,rair,rvap,cvap
      real co18prec,co18run,cdeutprec,cdeutrun,xt,mix_ave
      real true_depth

      integer max_dep,ix1,iy1,n_trace,i_area,lcount,nspin
      integer iin,iout,numpts,num_tra,ktau,iupto,ilake,jlake
      integer depth,depth_a,ktauwan,mixmax_a

      logical bndry_flag,iceflag,s_flag,o18flag,deutflag,ar_flag
      logical snow_flag_a,wb_flag,melt_flag_a
      real tempinit, deutinit, o18init !Ashling
!**********************************************************************
!**********************************************************************
! Lake specific parameters ********************************************

      parameter (oblq = 35.0)           ! obliquity 23.4
      parameter (xlat = -4.59)          ! latitude (negative for South) -6.30
      parameter (xlon = 137.44)         ! longitude (negative for West) 29.5
      parameter (gmt = +3)              ! local time relative to gmt in hours
      parameter (max_dep = 700.)         ! depth of lake at sill in meters 570.
      parameter (basedep = 0.)          ! elevation of basin bottom in meters (elevation, used for water balance) 733.
      parameter (b_area = 131590.4)    ! area of catchment+lake in hectares 23100000.
      parameter (cdrn = 0.00275)        ! neutral drag coefficient 1.8 HAD 1.7GISS 1.2CCSM 2.0e-3
      parameter (eta = 0.065)             ! shortwave extinction coefficient (1/m)
      parameter (f=0.3)                 ! fraction of advected air over lake 0.1
      parameter (alb_slush = 0.4)       ! albedo of melting snow
      parameter (alb_snow = 0.7)        ! albedo of non-melting snow

      parameter (depth_begin = 10.)     ! prescribed depth in meters (initialization only - this will always be the same) 570.
      parameter (salty_begin = 15.)     ! prescribed salinity in ppt 0.0

      parameter (o18air = -14.0)         ! d18O of air above lake
      parameter (deutair = -96.)       ! dD of air above lake

      co18prec(xt) = 0.3562*xt-21.079   ! function to calc o18prec
      co18run(xt) = 0.3562*xt-21.079    ! function to calc o18run
      cdeutprec(xt) = 2.5915*xt-161.35  ! function to calc deutprec
      cdeutrun(xt) = 2.5915*xt-161.35   ! function to calc deutrun

      real area(max_dep)
      data area/700*58590.4/           ! lake area in hectares by depth 570*3290000.

      !integer, parameter :: hyplength=1500
      !real, dimension(hyplength):: area
      !open(unit=100, file='hypsograph.txt')
      !read(100,*) area
      !close(unit=100) ! for orderly closing

      ! Removed from the model, for easier access
      character(38) :: datafile='input/WB_C5_input.txt'
      parameter (tempinit= -5.9) ! temperature to initialize lake at in INIT_LAKE subroutine 23.0
      parameter (deutinit= 24.0)! dD to initialize lake at in INIT_LAKE subroutine
      parameter (o18init= 3.7)  ! d18O to initialize lake at in INIT_LAKE subroutine
! Simulation specific parameters **************************************

      parameter (nspin = 10)              ! number of years for spinup
      parameter (bndry_flag = .false.)    ! true for explict boundry layer computations; presently only for sigma coord climate models
      parameter (sigma = 0.96)            ! sigma level for boundary flag
      parameter (wb_flag = .true.)       ! true for variable lake depth
      parameter (iceflag = .true.)        ! true for variable ice cover
      parameter (s_flag = .true.)        ! true for variable salinity
      parameter (o18flag = .false.)       ! true for variable d18O
      parameter (deutflag = .false.)      ! true for variable dD
      parameter (z_screen = 5.0)          ! height of met inputs

!**********************************************************************
! Other parameters DO NOT CHANGE without good reason for doing so******

      parameter (dz=1.0)                ! vertical layer thickness in m
      parameter (dt = 1.*60.*30.)       ! model time step in seconds
      parameter (zo=0.0004)             ! water surface roughness length
      parameter (n_trace=3)             ! model does 3 tracers (o18, D, sal)
      parameter (delta=5.67e-8)         ! s-b constant
      parameter (rhowat = 1000.)        ! density of water
      parameter (rhosnow = 330.)        ! density of snow
      parameter (rhoice = 917.)         ! density of ice
      parameter (snowcut = 2.2)         ! temp at which precip is snow
      parameter (cpair = 938)           ! specific heat capacity dry air 1.00464e3
      parameter (rair = 190.0)         ! specific gas constant dry air 287.05
      parameter (rvap = 461.495)        ! specific gas constant water vapor
      parameter (cvap = 1.81e3)         ! specific heat capacity water vapor
      parameter (Le = 2.45e6)           ! latent heat of vaporization water
      parameter (Lei = 2.5e6)           ! latent heat of vaporization ice
      parameter (ca = 6.11e2)           ! constant for Teten's formula
      parameter (cb = 273.16)           ! constant for Teten's formula
      parameter (c72 = 17.269)          ! constant for Teten over water
      parameter (c73 = 35.86)           ! constant for Teten over water
      parameter (c70 = 21.874)          ! constant for Teten over ice
      parameter (c71 = 7.66)            ! constant for Teten over ice
      parameter (emis = 0.95)           ! longwave emmisivity 0.97
      parameter (fusion=3.34e5)         ! latent heat of fusion
      parameter (surf = 1.0)            ! surface thickness over which the flux exchange occurs in water
      parameter (fracmin= 0.01)         ! min ice thick in meters
      parameter (fraclim = 0.02)        ! min ice fraction
      parameter (qwtau = 86400.)        ! D. Pollard sub-ice time constant
      parameter (cpw_ice = 4200.)       ! heat capacity of ice
      parameter (condw = .58)           ! conduction coefficinet for water
      parameter (beta=0.4)              ! frac of solar rad abs in water surface layer
      parameter (fsol1=0.5)             ! sw weighting for surface albedo
      parameter (fsol2=0.5)             ! lw weighting for surface albedo
      parameter (sical0=0.6)            ! sw albedo for sea ice
      parameter (sical1=0.4)            ! lw albedo for sea ice
      parameter (kv=0.4)                ! vonkarman constant
      parameter (dm=1.38889E-07)        ! thermal molecular diffusivity of water
      parameter (pi=3.141592654)        !
      parameter (snocrit = 0.05)        ! for albedo, in m
      parameter (dexch  =1.0)    ! fixed depth (m) over which isotope exchange occurs
      parameter (alphak = 0.994)        ! fractionation coeff
      parameter (lamisw = 3.0)          ! extinction coef for sw thru ice
      parameter (lamilw = 20.)          ! extinction coef for lw thru ice
      parameter (lamssw = 3.0)          ! extinction coef for sw thru snow
      parameter (lamslw = 20.)          ! extinction coef for lw thru snow
      parameter (afrac1 = 0.7)          ! fraction of light in visible band for ice model
      parameter (afrac2 = 0.3)          ! fraction of light in infrared band for ice model
      parameter (condi = 2.3)           ! thermal conductivity of ice
      parameter (conds = 0.31)          ! thermal conductivity of snow
      parameter (raddeg = 0.0174533)    ! radians per degree
      parameter (dpd = 360./687.)       ! degrees per day 365
      parameter (grav = 3.71)           ! gravity 9.80616

!**********************************************************************

! Common definitions
      common /ldata/ lcount, iin, iout, numpts , num_tra, ktau, iupto
      common mixing, o18snow_a,deutsnow_a
      common psum_a, depth_a
      common trace(max_dep,n_trace), trace_i(max_dep,n_trace)
      common trace_a (max_dep, n_trace),surfarea_a
      common d_fraca, tempi_a, hice_a, hsnow_a
      common salty_a, mixmax_a, fraci_a
      common temp_a (max_dep), snow_flag_a, melt_flag_a
      common ktauwan (12),ti_a(max_dep)
      common mix_ave, tsurf_ave, fice_ave
      common evap_ave, hice_ave, hsnow_ave, o18_ave
      common deut_ave, runout_sum, temp_ave(max_dep)

!**********************************************************************
