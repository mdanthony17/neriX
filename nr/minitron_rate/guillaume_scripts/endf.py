import re, math
from scipy import special

version = '0.1.0'

_data_directory = '/Users/Matt/Desktop/Xenon/neriX/nr/minitron_rate/guillaume_scripts/data/'
_elements = None
_elements_db = '0-elements.dat'
_particles = None
_particles_db = '0-particles.dat'

class InterpolationError(Exception):
	"""Exception raised for interpolation errors

	Attributes:
	    value -- value causing the exception
	    msg   -- explanation of the error
	"""
	def __init__(self, value, msg):
		self.value = value
		self.msg = msg

	def __str__(self):
		return '%0.4e, %s' % (self.value, self.msg)

class FormatError(Exception):
	"""Exception raised for ENDF format errors

	Attributes:
	    msg   -- explanation of the error
	"""
	def __init__(self, msg):
		self.msg = msg

	def __str__(self):
		return self.msg

class InitializationError(Exception):
	"""Exception raised for class initialization errors

	Attributes:
	    msg   -- explanation of the error
	"""
	def __init__(self, msg):
		self.msg = msg

	def __str__(self):
		return self.msg

class ReactionError(Exception):
	"""Exception raised for nuclear reaction errors

	Attributes:
	    msg   -- explanation of the error
	"""
	def __init__(self, msg):
		self.msg = msg

	def __str__(self):
		return self.msg

class Element():
	def __init__(self, symbol='', name='', atomic_number=0, isotopic_abundances=None):
		global _elements

		if symbol and name and atomic_number and isotopic_abundances:
			self.symbol = symbol
			self.name = name
			self.atomic_number = int(atomic_number)
			self.isotopic_abundances = isotopic_abundances
		elif not (symbol or name or atomic_number):
			raise InitializationError('no symbol, name or atomic_number!')
		else:
			for element in _elements:
				if (not symbol or symbol == element.symbol) and (not name or name == element.name) and (not atomic_number or atomic_number == element.atomic_number):
					self.symbol = element.symbol
					self.name = element.name
					self.atomic_number = element.atomic_number
					self.isotopic_abundances = element.isotopic_abundances

	def __str__(self):
		return '%s' % (self.symbol)

	def __repr__(self):
		return '<endf.Element(%s)>' % (self.name)

class Isotope():
	def __init__(self, symbol='', name='', atomic_number=0, mass_number=0):
		global _elements

		if symbol and name and atomic_number and mass_number:
			self.symbol = symbol
			self.name = name
			self.atomic_number = int(atomic_number)
			self.mass_number = int(mass_number)
		elif not ((symbol or name or atomic_number) and mass_number):
			raise InitializationError('no symbol, name or atomic_number or no mass_number!')
		else:
			for element in _elements:
				if (not symbol or symbol == element.symbol) and (not name or name == element.name) and (not atomic_number or atomic_number == element.atomic_number):
					self.symbol = element.symbol
					self.name = element.name
					self.atomic_number = element.atomic_number
					self.mass_number = mass_number
	
	def __str__(self):
		return '%s%d' % (self.symbol, self.mass_number)

	def __repr__(self):
		return '<endf.Isotope(%s)>' % (self.name)

	def __eq__(self, i):
		if isinstance(i, Isotope):
			return (self.symbol == i.symbol) and (self.atomic_number == i.atomic_number) and (self.mass_number == i.mass_number)
		else:
			return False

class Particle():
	def __init__(self, symbol='', name='', charge=None, nb_nucleons=None):
		global _particles

		if symbol and name and charge and nb_nucleons:
			self.symbol = symbol
			self.name = name
			self.charge = int(charge)
			self.nb_nucleons = int(nb_nucleons)
		elif not (symbol or name):
			raise InitializationError('no symbol or name!')
		else:
			for particle in _particles:
				if (not symbol or symbol == particle.symbol) and (not name or name == particle.name):
					self.symbol = particle.symbol
					self.name = particle.name
					self.charge = particle.charge
					self.nb_nucleons = particle.nb_nucleons

	def __str__(self):
		return self.symbol

	def __repr__(self):
		return '<endf.Particle(%s)>' % (self.name)

	def __eq__(self, p):
		if isinstance(p, Particle):
			return self.symbol == p.symbol
		else:
			return False

class Reaction():
	def __init__(self, reactants, products):
		self.reactants = reactants
		self.products = products

	def is_binary(self):
		return len(self.reactants) == 2

	def is_two_products(self):
		return len(self.products) == 2

	def is_elastic(self):
		return isinstance(self, ElasticScattering)

	def is_inelastic(self):
		return isinstance(self, InelasticScattering)

	def kinematic_threshold(self):
		if self.is_binary() and self.is_two_product():
			mx = self.reactants[0].mass
			my = self.products[0].mass
			mY = self.products[1].mass
			kinematic_threshold = -(my+mY)/(my+mY-mx)*Q
		else:
			kinematic_threshold = None

		return kinematic_threshold

	def __str__(self):
		pass

class ElasticScattering(Reaction):
	def __init__(self, projectile, target):
		Reaction.__init__(self, [projectile, target], [projectile, target])

class InelasticScattering(Reaction):
	def __init__(self, projectile, target, level):
		self.level = level
		Reaction.__init__(self, [projectile, target], [projectile, target])

class NeutronProduction(Reaction):
	def __init__(self, projectile, target):
		if isinstance(projectile, Particle):
			new_atomic_number = projectile.charge+target.atomic_number
			new_mass_number = projectile.nb_nucleons+target.mass_number
		elif isinstance(projectile, Isotope):
			new_atomic_number = projectile.atomic_number+target.atomic_number
			new_mass_number = projectile.mass_number+target.mass_number

		product_nucleus = Isotope(atomic_number=new_atomic_number, mass_number=new_mass_number)

		Reaction.__init__(self, [projectile, target], [Particle('n'), product_nucleus])

def endf_float(s):
	return float(re.sub('([ -]\d\.\d+)([+-]\d+)', lambda m: m.group(1)+'e'+m.group(2), s))
	
class Parser():
	def __init__(self, lines):
		self.lines = lines
		self.current_line = 0

	def read_text_record(self):
		"""Read an ENDF TEXT record

		ENDF Syntax:
		   [MAT, MF, MT / HL] TEXT
		"""
		line = self.lines[self.current_line].rstrip()
		self.current_line += 1

		text_record = []
		text_record.append(line[0:66])
		text_record.append(int(line[66:70]))
		text_record.append(int(line[70:72]))
		text_record.append(int(line[72:75]))
		text_record.append(int(line[75:80]))

		return tuple(text_record)

	def read_head_record(self):
		"""Read an ENDF HEAD record

		ENDF Syntax:
		   [MAT, MF, MT / ZA, AWR, L1, L2, N1, N2] CONT
		"""
		line = self.lines[self.current_line].rstrip()
		self.current_line += 1

		head_record = []
		head_record.append(endf_float(line[0:11]))
		head_record.append(endf_float(line[11:22]))
		head_record.append(int(line[22:33]))
		head_record.append(int(line[33:44]))
		head_record.append(int(line[44:55]))
		head_record.append(int(line[55:66]))
		head_record.append(int(line[66:70]))
		head_record.append(int(line[70:72]))
		head_record.append(int(line[72:75]))
		head_record.append(int(line[75:80]))

		return tuple(head_record)

	def read_cont_record(self):
		"""Read an ENDF CONT record

		ENDF Syntax:
		   [MAT, MF, MT / C1, C2, L1, L2, N1, N2] CONT
		"""
		line = self.lines[self.current_line].rstrip()
		self.current_line += 1

		cont_record = []
		cont_record.append(endf_float(line[0:11]))
		cont_record.append(endf_float(line[11:22]))
		cont_record.append(int(line[22:33]))
		cont_record.append(int(line[33:44]))
		cont_record.append(int(line[44:55]))
		cont_record.append(int(line[55:66]))
		cont_record.append(int(line[66:70]))
		cont_record.append(int(line[70:72]))
		cont_record.append(int(line[72:75]))
		cont_record.append(int(line[75:80]))

		return tuple(cont_record)

	def read_send_record(self):
		"""Read an ENDF SEND record (section end)

		ENDF Syntax:
		   [MAT, MF, 0 / 0.0, 0.0, 0, 0, 0, 0] SEND
		"""
		line = self.lines[self.current_line].rstrip()
		self.current_line += 1

		send_record = []
		send_record.append(endf_float(line[0:11]))
		send_record.append(endf_float(line[11:22]))
		send_record.append(int(line[22:33]))
		send_record.append(int(line[33:44]))
		send_record.append(int(line[44:55]))
		send_record.append(int(line[55:66]))
		send_record.append(int(line[66:70]))
		send_record.append(int(line[70:72]))
		send_record.append(int(line[72:75]))
		send_record.append(int(line[75:80]))

		return tuple(send_record)

	def read_list_record(self):
		"""Read an ENDF LIST record

		ENDF Syntax:
		   [MAT, MF, MT / C1, C2, L1, L2, NPL, N2 / B_n] LIST
		"""
		line = self.lines[self.current_line].rstrip()
		self.current_line += 1

		list_record = []
		list_record.append(endf_float(line[0:11]))
		list_record.append(endf_float(line[11:22]))
		list_record.append(int(line[22:33]))
		list_record.append(int(line[33:44]))
		list_record.append(int(line[44:55]))
		list_record.append(int(line[55:66]))
		list_record.append(int(line[66:70]))
		list_record.append(int(line[70:72]))
		list_record.append(int(line[72:75]))
		list_record.append(int(line[75:80]))

		npl = list_record[4]

		bn = []
		for b_i in range(0, npl):
			if b_i % 6 == 0:
				line = self.lines[self.current_line].rstrip()
				self.current_line += 1
				i = 0
			bn.append(endf_float(line[i*11:(i+1)*11]))
			i += 1
		list_record.append(tuple(bn))

		return tuple(list_record)

	def read_tab1_record(self):
		"""Read an ENDF TAB1 record

		ENDF Syntax:
		   [MAT, MF, MT / C1, C2, L1, L2, NR, NP / x_int / y(x)] TAB1
		"""
		line = self.lines[self.current_line].rstrip()
		self.current_line += 1

		tab1_record = []
		tab1_record.append(endf_float(line[0:11]))
		tab1_record.append(endf_float(line[11:22]))
		tab1_record.append(int(line[22:33]))
		tab1_record.append(int(line[33:44]))
		tab1_record.append(int(line[44:55]))
		tab1_record.append(int(line[55:66]))
		tab1_record.append(int(line[66:70]))
		tab1_record.append(int(line[70:72]))
		tab1_record.append(int(line[72:75]))
		tab1_record.append(int(line[75:80]))

		nr = tab1_record[4]
		np = tab1_record[5]

		nbtm_intm = []
		for r_i in range(0, nr):
			if r_i % 3 == 0:
				line = self.lines[self.current_line].rstrip()
				self.current_line += 1
				i = 0
			nbtm_intm.append((int(line[i*11:(i+1)*11]), int(line[(i+1)*11:(i+2)*11])))
			i += 2
		tab1_record.append(tuple(nbtm_intm))

		xint_fx = []
		for p_i in range(0, np):
			if p_i % 3 == 0:
				line = self.lines[self.current_line].rstrip()
				self.current_line += 1
				i = 0
			xint_fx.append((endf_float(line[i*11:(i+1)*11]), endf_float(line[(i+1)*11:(i+2)*11])))
			i += 2
		tab1_record.append(tuple(xint_fx))

		return tuple(tab1_record)

	def read_tab2_record(self):
		"""Read an ENDF TAB2 record

		ENDF Syntax:
		   [MAT, MF, MT / C1, C2, L1, L2, NR, NZ / Z_int] TAB2
		"""
		line = self.lines[self.current_line].rstrip()
		self.current_line += 1

		tab2_record = []
		tab2_record.append(endf_float(line[0:11]))
		tab2_record.append(endf_float(line[11:22]))
		tab2_record.append(int(line[22:33]))
		tab2_record.append(int(line[33:44]))
		tab2_record.append(int(line[44:55]))
		tab2_record.append(int(line[55:66]))
		tab2_record.append(int(line[66:70]))
		tab2_record.append(int(line[70:72]))
		tab2_record.append(int(line[72:75]))
		tab2_record.append(int(line[75:80]))

		nr = tab2_record[4]
		nz = tab2_record[5]

		nbtm_intm = []
		for r_i in range(0, nr):
			if r_i % 3 == 0:
				line = self.lines[self.current_line].rstrip()
				self.current_line += 1
				i = 0
			nbtm_intm.append((int(line[i*11:(i+1)*11]), int(line[(i+1)*11:(i+2)*11])))
			i += 2
		tab2_record.append(tuple(nbtm_intm))

		return tuple(tab2_record)

	def read_file3_section(self):
		"""Read an ENDF FILE3 section

		ENDF Syntax:
		   [MAT, 3, MT / ZA, AWR, 0, 0, 0, 0] HEAD
		   [MAT, 3, MT / QM, QI, 0, LR, NR, NP / E_int / sigma(E)] TAB1
		   [MAT, 3, 0 / 0.0, 0.0, 0, 0, 0, 0] SEND
		"""
		file3_section = []
		file3_section.append(self.read_head_record())
		file3_section.append(self.read_tab1_record())
		file3_section.append(self.read_send_record())

		return tuple(file3_section)

	def read_file4_section(self):
		"""Read an ENDF FILE4 section

		ENDF Syntax:
		   [MAT, 4, MT / ZA, AWR, 0, LTT, 0, 0] HEAD (LTT=1)
		   [MAT, 4, MT / 0.0, AWR, LI, LCT, 0, 0] CONT (LI=0)
		   [MAT, 4, MT / 0.0, 0.0, 0, 0, NR, NE / E_int] TAB2
		   [MAT, 4, MT / T, E_1, LT, 0, NL, 0 / a_l(E_1)] LIST
		   [MAT, 4, MT / T, E_2, LT, 0, NL, 0 / a_l(E_2)] LIST
		   ...
		   [MAT, 4, MT / T, E_NE, LT, 0, NL, 0 / a_l(E_NE)] LIST
		   [MAT, 4, 0 / 0.0, 0.0, 0, 0, 0, 0] SEND
		"""
		file4_section = []
		file4_section.append(self.read_head_record())
		file4_section.append(self.read_cont_record())
		ltt = file4_section[0][3]
		li = file4_section[1][2]
		lct = file4_section[1][3]
		if ltt == 1 and li == 0:
			file4_section.append(self.read_tab2_record())
			ne = file4_section[2][5]
			coefficients = []
			for i in range(0, ne):
				coefficients.append(self.read_list_record())
			file4_section.append(tuple(coefficients))
			file4_section.append(self.read_send_record())
		else:
			raise FormatError('unimplemented: FILE4: LTT=%d, LI=%d!' % (ltt, li))

		return tuple(file4_section)

	def read_file6_section(self):
		"""Read an ENDF FILE6 section

		ENDF Syntax:
		   [MAT, 6, MT / ZA, AWR, 0, LCT, NK, 0] HEAD
		   [MAT, 6, MT / ZAP, AWP, LIP, LAW, NR, NP / E_int / y_i(E)] TAB1
		   <LAW-dependent records for product 1>
		   <LAW-dependent records for product 2>
		   ...
		   <LAW-dependent records for product NK>
		   [MAT, 6, 0 / 0.0, 0.0, 0, 0, 0, 0] SEND

		   (LAW=2)
		   [MAT, 6, MT / 0.0, 0.0, 0, 0, NR, NE / E_int ] TAB2
		   [MAT, 6, MT / 0.0, E_1, LANG, 0, NW, NL/ a_l(E_1)] LIST
		   [MAT, 6, MT / 0.0, E_2, LANG, 0, NW, NL/ a_l(E_2)] LIST
		   ...
		   [MAT, 6, MT / 0.0, E_NE, LANG, 0, NW, NL/ a_l(E_NE)] LIST

		   (LAW=4)
		"""
		file6_section = []
		file6_section.append(self.read_head_record())
		nk = file6_section[0][4]
		products = []
		for k in range(0, nk):
			product_k = []
			product_k.append(self.read_tab1_record())
			law = product_k[0][3]
			if law == 2:
				product_k.append(self.read_tab2_record())
				ne = product_k[1][5]
				coefficients = []
				for i in range(0, ne):
					coefficients.append(self.read_list_record())
				product_k.append(tuple(coefficients))
			elif law ==4:
				pass
			else:
				raise FormatError('unimplemented: FILE6: LAW=%d!' % law)
			products.append(tuple(product_k))
		file6_section.append(tuple(products))
		file6_section.append(self.read_send_record())

		return tuple(file6_section)

class Interpolation1D():
	"""One dimensional interpolation class

	Attributes:
	    nr        -- number of interpolation ranges
	    nbtm_intm -- interpolation table
	    np        -- number of pairs
	    xint_yx   -- pair table
	"""
	def __init__(self, nr, nbtm_intm, np, xint_yx):
		self.nr = nr
		self.nbtm_intm = nbtm_intm
		self.np = np
		self.xint_yx = xint_yx

	def __call__(self, x):
		# find the right limit index for the interpolation
		i = 0
		while i < len(self.xint_yx) and x > self.xint_yx[i][0]:
			i += 1

		# find which interpolation range it corresponds to
		m = 0
		while m < len(self.nbtm_intm) and i > self.nbtm_intm[m][0]:
			m += 1

		if i == 0 or i == len(self.xint_yx):
			raise InterpolationError(x, 'value out of range!')

		x0, y0 = self.xint_yx[i-1]
		x1, y1 = self.xint_yx[i]

		if self.nbtm_intm[m][1] == 2:
			# linear-linear interpolation
			yx = y0 + (x-x0)*(y1-y0)/(x1-x0)
		elif self.nbtm_intm[m][1] == 3:
			# linear-log interpolation
			yx = y0 + math.log(x/x0)*(y1-y0)/math.log(x1/x0)
		elif self.nbtm_intm[m][1] == 5:
			# log-log interpolation
			yx = y0 * math.exp(math.log(x/x0)*math.log(y1/y0)/math.log(x1/x0))
		elif self.nbtm_intm[m][1] == 6:
			# special charged-particle cross section interpolation
			t = 0.
			b = math.log((y1*x1)/(y0*x0))/(1./math.sqrt(x0-t)-1./math.sqrt(x1-t))
			a = math.exp(b/math.sqrt(x0-t))*y0*x0
			yx = a/x*math.exp(-b/math.sqrt(x-t))
		else:
			raise InterpolationError(x, 'interpolation mode %d not supported!' % self.nbtm_intm[m][1])

		return yx

class LegendrePolynomialExpansion1D():
	def __init__(self, al):
		self.al = al
		self.f = sum([0.5]+[(2.*l+1.)/2.*a*special.legendre(l) for l,a in zip(range(1,1+len(self.al)), self.al)])

	def __call__(self, mu):
		return self.f(mu)

class Interpolation2D():
	"""Two dimensional interpolation class

	Attributes:
	    nr        -- number of interpolation ranges
	    nbtm_intm -- interpolation table
	    nz        -- number of 1D function pairs
	    zint_yxz  -- 1D function pair table
	"""
	def __init__(self, nr, nbtm_intm, nz, zint_yxz):
		self.nr = nr
		self.nbtm_intm = nbtm_intm
		self.nz = nz
		self.zint_yxz = zint_yxz

	def __call__(self, x, z):
		# find the right limit index for the interpolation
		i = 0
		while i < len(self.zint_yxz) and z > self.zint_yxz[i][0]:
			i += 1

		# find which interpolation range it corresponds to
		m = 0
		while m < len(self.nbtm_intm) and i > self.nbtm_intm[m][0]:
			m += 1

		if i == 0 or i == len(self.zint_yxz):
			raise InterpolationError(z, 'value out of range!')

		z0, y0 = self.zint_yxz[i-1][0], self.zint_yxz[i-1][1](x)
		z1, y1 = self.zint_yxz[i][0], self.zint_yxz[i][1](x)

		if self.nbtm_intm[m][1] == 2:
			# linear-linear interpolation
			yxz = y0 + (z-z0)*(y1-y0)/(z1-z0)
		elif self.nbtm_intm[m][1] == 3:
			# linear-log interpolation
			yxz = y0 + math.log(z/z0)*(y1-y0)/math.log(z1/z0)
		elif self.nbtm_intm[m][1] == 5:
			# log-log interpolation
			yxz = y0 * math.exp(math.log(z/z0)*math.log(y1/y0)/math.log(z1/z0))
		else:
			raise InterpolationError(z, 'interpolation mode %d not supported!' % self.nbtm_intm[m][1])

		return yxz

class Sigma(Interpolation1D):
	def __init__(self, reaction):
		global _data_directory

		self.reaction = reaction

		if reaction.is_binary() and reaction.is_two_products():
			if reaction.is_elastic():
				endf_filename = '%s-%s,el-sig.dat' % (str(reaction.reactants[1]).lower(), str(reaction.reactants[0]).lower())
			elif reaction.is_inelastic():
				endf_filename = '%s-%s,inl-l%d-sig.dat' % (str(reaction.reactants[1]).lower(), str(reaction.reactants[0]).lower(), reaction.level)
			else:
				endf_filename = '%s-%s,%s-sig.dat' % (str(reaction.reactants[1]).lower(), str(reaction.reactants[0]).lower(), str(reaction.products[0]).lower())

			parser = Parser(open(_data_directory+endf_filename, 'r').readlines())
			file3_section = parser.read_file3_section()

			tab1_record = file3_section[1]

			nr = tab1_record[4]
			nbtm_intm = tab1_record[10]
			np = tab1_record[5]
			xint_yx = tab1_record[11]

			if tab1_record[1] < 0 and (6 in map(lambda p: p[1], nbtm_intm)):
				raise ReactionError('reaction has negative Q-value, interpolation mode 6 not valid!')

			Interpolation1D.__init__(self, nr, nbtm_intm, np, xint_yx)
		else:
			raise ReactionError('unknown reaction!')

class DSigmaDTheta(Interpolation2D):
	def __init__(self, reaction):
		global _data_directory

		self.reaction = reaction

		if reaction.is_binary() and reaction.is_two_products():
			if reaction.is_elastic():
				endf_filename = '%s-%s,el-da.dat' % (str(reaction.reactants[1]).lower(), str(reaction.reactants[0]).lower())
			elif reaction.is_inelastic():
				endf_filename = '%s-%s,inl-l%d-da.dat' % (str(reaction.reactants[1]).lower(), str(reaction.reactants[0]).lower(), reaction.level)
			else:
				raise ReactionError('unknown reaction!')

			parser = Parser(open(_data_directory+endf_filename, 'r').readlines())
			file4_section = parser.read_file4_section()

			ltt = file4_section[0][3]
			li = file4_section[1][2]
			lct = file4_section[1][3]
			if ltt == 1 and li == 0:
				nr = file4_section[2][4]
				nbtm_intm = file4_section[2][10]
				ne = file4_section[2][5]
				eint_fmue = []
				for i in range(0, ne):
					eint = file4_section[3][i][1]
					al = file4_section[3][i][10]
					eint_fmue.append((eint, LegendrePolynomialExpansion1D(al)))

			Interpolation2D.__init__(self, nr, nbtm_intm, ne, eint_fmue)

			self.sigma = Sigma(reaction)
		else:
			raise ReactionError('unknown reaction!')
	
	def __call__(self, angle, energy):
		return (self.sigma(energy)/(2.*math.pi))*Interpolation2D.__call__(self, angle, energy)

class DSigmaDThetaForProduct(Interpolation2D):
	def __init__(self, reaction, product):
		global _data_directory

		self.reaction = reaction
		if isinstance(product, Particle):
			self.desired_zap = 1000.*product.charge+product.nb_nucleons
		else:
			self.desired_zap = 1000.*product.atomic_number+product.mass_number

		if reaction.is_binary() and reaction.is_two_products():
			if reaction.is_elastic():
				endf_filename = '%s-%s,el-dade.dat' % (str(reaction.reactants[1]).lower(), str(reaction.reactants[0]).lower())
			elif reaction.is_inelastic():
				endf_filename = '%s-%s,inl-l%d-dade.dat' % (str(reaction.reactants[1]).lower(), str(reaction.reactants[0]).lower(), reaction.level)
			else:
				endf_filename = '%s-%s,%s-dade.dat' % (str(reaction.reactants[1]).lower(), str(reaction.reactants[0]).lower(), str(reaction.products[0]).lower())

			parser = Parser(open(_data_directory+endf_filename, 'r').readlines())
			file6_section = parser.read_file6_section()

			desired_product = None
			for product_record in file6_section[1]:
				if product_record[0][0] == self.desired_zap:
					desired_product = product_record
					break

			if not desired_product:
				raise ReactionError('angular distribution of product %s not present!' % product)

			lct = file6_section[0][3]
			nk = file6_section[0][4]

			tab1_record = desired_product[0]

			nr = tab1_record[4]
			nbtm_intm = tab1_record[10]
			np = tab1_record[5]
			xint_yx = tab1_record[11]
			law = tab1_record[3]

			self.product_yield = Interpolation1D(nr, nbtm_intm, np, xint_yx)

			if law == 2:
				tab2_record = desired_product[1]
				nr = tab2_record[4]
				nbtm_intm = tab2_record[10]
				ne = tab2_record[5]
				eint_fmue = []
				for i in range(0, ne):
					eint = desired_product[2][i][1]
					al = desired_product[2][i][10]
					eint_fmue.append((eint, LegendrePolynomialExpansion1D(al)))

			Interpolation2D.__init__(self, nr, nbtm_intm, ne, eint_fmue)

			self.sigma = Sigma(reaction)
		else:
			raise ReactionError('unknown reaction!')
	
	def __call__(self, angle, energy):
		return (self.sigma(energy)*self.product_yield(energy)/(2.*math.pi))*Interpolation2D.__call__(self, angle, energy)

def _read_elements():
	"""
	"""
	global _data_directory, _elements_db

	lines = open(_data_directory+_elements_db).readlines()
	elements = []
	for line in lines:
		elements.append(Element(*(line.split()[0:3]+[dict(map(lambda s: s.split(':'), line.split()[3:]))])))

	return elements

def _read_particles():
	"""
	"""
	global _data_directory, _particles_db

	lines = open(_data_directory+_particles_db).readlines()
	particles = []
	for line in lines:
		 particles.append(Particle(*line.split()))

	return particles

def _initialize():
	"""
	"""
	global _elements, _particles

	_elements = _read_elements()
	_particles = _read_particles()

_initialize()

