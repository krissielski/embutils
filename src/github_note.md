
## Enable Branch Protection on GitHub

1. Go to your repository on GitHub
2. Click **Settings** → **Branches**
3. Click **Add branch protection rule**
4. Set **Branch name pattern**: `main`
5. Enable:
   - ✓ **Require a pull request before merging**
   - ✓ **Require status checks to pass before merging**
   - ✓ **Require branches to be up to date before merging**
6. In the search box under "Status checks that are required", search for
   and add your job names:
   - `build-and-test (ubuntu-22.04, gcc)`
   - `build-and-test (ubuntu-22.04, clang)`
   - `build-and-test (ubuntu-24.04, gcc)`
   - `build-and-test (ubuntu-24.04, clang)`
   - `static-analysis`
7. Click **Create**





