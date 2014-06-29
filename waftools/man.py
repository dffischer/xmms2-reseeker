# man.py
# Compresses and installs manpages.
# Originally by Daniel Svensson <https://github.com/dsvensson>
# <dsvensson@gmail.com> for Abraca <http://abraca.github.io/Abraca

from waflib.Task import task_factory
from waflib.Errors import BuildError
from waflib.TaskGen import feature,before_method
from os.path import basename
from gzip import GzipFile


def gzip_func(task):
    infile = task.inputs[0].abspath()
    outfile = task.outputs[0].abspath()

    with open(infile, 'rb') as input, open(outfile, 'wb') as outfile:
      with GzipFile(basename(infile), fileobj=outfile) as output:
        output.write(input.read())

task_factory('man', gzip_func, color='BLUE')

@feature('man')
@before_method('process_source')
def process_man(self):
    source = self.to_nodes(getattr(self, 'source', []))
    self.source = []

    section = getattr(self, 'section', None)

    for node in source:
        if not node:
            raise BuildError('cannot find input file %s for processing' % x)

        s = section or "." in node.name and node.name.rsplit(".", 1)[1]
        if not s:
            raise BuildError('cannot determine man section from filename')

        out = self.path.find_or_declare(node.name + '.gz')

        tsk = self.create_task('man')
        tsk.set_inputs(node)
        tsk.set_outputs(out)

        self.bld.install_files('${MANDIR}/man%s' % s, out)

def options(opt):
  opt.load('gnu_dirs')

def configure(cnf):
  cnf.load('gnu_dirs')
