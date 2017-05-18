import fit_nr


# "okay" afterwards means it is fine to
# run without suppression (2E6*8 loops)
l_jobs_to_run = []
l_jobs_to_run.append([0.345, 3000]) # okay
l_jobs_to_run.append([0.345, 3500]) # okay
l_jobs_to_run.append([0.345, 4500]) # okay
l_jobs_to_run.append([0.345, 5300]) # okay
l_jobs_to_run.append([1.054, 3000]) # okay
l_jobs_to_run.append([1.054, 3500]) # okay
l_jobs_to_run.append([1.054, 4500]) # okay
l_jobs_to_run.append([1.054, 5300]) # okay
l_jobs_to_run.append([2.356, 3000]) # okay
l_jobs_to_run.append([2.356, 3500]) # okay
l_jobs_to_run.append([2.356, 4500]) # okay
l_jobs_to_run.append([2.356, 5300]) # okay


for cathode_setting, degree_setting in l_jobs_to_run:

    print '\n\n\n\nStarting %.3f kV and %d degrees\n\n\n' % (cathode_setting, degree_setting)

    d_coincidence_data = {}
    d_coincidence_data['degree_settings'] = [degree_setting]
    d_coincidence_data['cathode_settings'] = [cathode_setting]

    test = fit_nr.fit_nr('s', d_coincidence_data, num_mc_events=2e6, l_gpus=[0, 1, 2, 3, 4, 5], num_loops=4*2)

    # for 2E6*8 events do not need to suppress likelihood
    #test.suppress_likelihood()
    test.run_mcmc(num_steps=2000, num_walkers=256)

    test.close_workers()

