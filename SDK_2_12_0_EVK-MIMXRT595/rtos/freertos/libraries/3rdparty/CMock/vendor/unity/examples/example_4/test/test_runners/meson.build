###################################################################################
#                                                                                 #
# NAME: meson.build                                                               #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################

cases = [['TestProductionCode_Runner.c',  join_paths('..' ,'TestProductionCode.c')], 
        ['TestProductionCode2_Runner.c', join_paths('..' ,'TestProductionCode2.c')]]

test('Running: 01-test-case', executable('01-test-case', cases[0], dependencies: [ a_dep, unity_dep ]))
test('Running: 02-test-case', executable('02-test-case', cases[1], dependencies: [ b_dep, unity_dep ]))
